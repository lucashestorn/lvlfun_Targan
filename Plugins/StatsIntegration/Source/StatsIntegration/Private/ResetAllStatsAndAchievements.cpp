// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "ResetAllStatsAndAchievements.h"
#include "Engine.h"

void UResetAllStatsAndAchievements::UnregisterAllCallbacks()
{
	UserStatsStored.Unregister();
	UserAchievementsStored.Unregister();
	UserStatsReceived.Unregister();
}

UResetAllStatsAndAchievements* UResetAllStatsAndAchievements::ResetUserStatsAndAchievements(bool bAchievementsToo)
{
	UResetAllStatsAndAchievements* selfRef = NewObject<UResetAllStatsAndAchievements>();

	selfRef->SteamSubsystem = static_cast<FOnlineSubsystemSteam*>(IOnlineSubsystem::Get());
	selfRef->bresetAchievementsToo = bAchievementsToo;
	selfRef->bFetchedStatsOrAchievements = false;

	selfRef->UserStatsStored.Register(selfRef, &UResetAllStatsAndAchievements::OnUserStatsStored);
	selfRef->UserAchievementsStored.Register(selfRef, &UResetAllStatsAndAchievements::OnUserAchievementsStored);
	selfRef->UserStatsReceived.Register(selfRef, &UResetAllStatsAndAchievements::OnUserStatsReceived);

	return selfRef;
}

void UResetAllStatsAndAchievements::Activate()
{
	if (SteamAPI_Init())
	{
		if (!SteamUserStats()->ResetAllStats(bresetAchievementsToo))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to reset stats and/or achievements!"));

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

void UResetAllStatsAndAchievements::OnUserStatsStored(UserStatsStored_t* pCallback)
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
				SteamUserStats()->RequestCurrentStats();
			}
		}
		else
		{
			onFailure.Broadcast(callbackResult);
			UnregisterAllCallbacks();
		}
	});
}

void UResetAllStatsAndAchievements::OnUserAchievementsStored(UserAchievementStored_t* pCallback)
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
				SteamUserStats()->RequestCurrentStats();
			}
		}
		else
		{
			onFailure.Broadcast(EResult::k_EResultFail);
			UnregisterAllCallbacks();
		}
	});
}

void UResetAllStatsAndAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback)
{
	uint64 gameID = pCallback->m_nGameID;
	EResult callbackResult = pCallback->m_eResult;

	SteamSubsystem->ExecuteNextTick([gameID, callbackResult, this] {
		if (gameID == SteamUtils()->GetAppID() && callbackResult == EResult::k_EResultOK)
		{
			SteamAPICall_t hSteamAPICall = SteamUserStats()->RequestGlobalAchievementPercentages();
			m_callRequestGlobalAchievementPercentages.Set(hSteamAPICall, this, &UResetAllStatsAndAchievements::OnRequestGlobalAchievementPercentages);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to receive stats!"));

			onFailure.Broadcast(callbackResult);
			UnregisterAllCallbacks();
		}
	});
}

void UResetAllStatsAndAchievements::OnRequestGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t* pCallback, bool bIOFailure)
{
	uint64 gameID = pCallback->m_nGameID;
	EResult callbackResult = pCallback->m_eResult;

	SteamSubsystem->ExecuteNextTick([gameID, callbackResult, this] {
		if (gameID == SteamUtils()->GetAppID())
		{
			if (callbackResult == EResult::k_EResultFail)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No global achievement percentages for the current AppId!"));
			}

			onSuccess.Broadcast(EResult::k_EResultOK);
			UnregisterAllCallbacks();
		}
		else
		{
			onFailure.Broadcast(callbackResult);
			UnregisterAllCallbacks();
		}
	});
}
