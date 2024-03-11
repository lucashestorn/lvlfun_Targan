// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "StoreStatsAndAchievements.h"
#include "Engine.h"
#include <string>

void UStoreStatsAndAchievements::UnregisterAllCallbacks()
{
	StoreUserStats.Unregister();
	StoreUserAchievements.Unregister();
}

UStoreStatsAndAchievements* UStoreStatsAndAchievements::StoreUserStatsAndAchievements()
{
	UStoreStatsAndAchievements* selfRef = NewObject<UStoreStatsAndAchievements>();

	selfRef->SteamSubsystem = static_cast<FOnlineSubsystemSteam*>(IOnlineSubsystem::Get());
	selfRef->bFetchedStatsOrAchievements = false;

	selfRef->StoreUserStats.Register(selfRef, &UStoreStatsAndAchievements::OnUserStatsStored);
	selfRef->StoreUserAchievements.Register(selfRef, &UStoreStatsAndAchievements::OnUserAchievementsStored);

	return selfRef;
}

void UStoreStatsAndAchievements::Activate()
{
	if (SteamAPI_Init())
	{
		if (!SteamUserStats()->StoreStats())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to store stats!"));

			onFailure.Broadcast(EResult::k_EResultFail);
			UnregisterAllCallbacks();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to initialize the Steam API!"));

		onFailure.Broadcast(EResult::k_EResultFail);
		UnregisterAllCallbacks();
	}
}

void UStoreStatsAndAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
{
	uint64 gameID = pCallback->m_nGameID;
	EResult callbackResult = pCallback->m_eResult;

	SteamSubsystem->ExecuteNextTick([gameID, callbackResult, this] {
		if (gameID == SteamUtils()->GetAppID() && callbackResult == k_EResultOK)
		{
			if (!bFetchedStatsOrAchievements)
			{
				bFetchedStatsOrAchievements = true;
			}
			else
			{
				onSuccess.Broadcast(callbackResult);
				UnregisterAllCallbacks();
			}
		}
		else
		{
			onFailure.Broadcast(callbackResult);
			UnregisterAllCallbacks();
		}
	});
}


void UStoreStatsAndAchievements::OnUserAchievementsStored(UserAchievementStored_t* pCallback)
{
	uint64 gameID = pCallback->m_nGameID;

	SteamSubsystem->ExecuteNextTick([gameID, this] {
		if (gameID == SteamUtils()->GetAppID())
		{
			if (!bFetchedStatsOrAchievements)
			{
				bFetchedStatsOrAchievements = true;
			}
			else
			{
				onSuccess.Broadcast(EResult::k_EResultOK);
				UnregisterAllCallbacks();
			}
		}
		else
		{
			onFailure.Broadcast(EResult::k_EResultFail);
			UnregisterAllCallbacks();
		}
	});
}

