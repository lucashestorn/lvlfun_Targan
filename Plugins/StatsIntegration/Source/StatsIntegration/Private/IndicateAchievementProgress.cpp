// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "IndicateAchievementProgress.h"
#include "Engine.h"

void UIndicateAchievementProgress::UnregisterAllCallbacks()
{
	UserStatsStored.Unregister();
	UserAchievementsStored.Unregister();
}

UIndicateAchievementProgress* UIndicateAchievementProgress::IndicateUserAchievementProgress(FString AchievementAPIName, int32 CurrentProgress, int32 MaxProgress)
{
	UIndicateAchievementProgress* selfRef = NewObject<UIndicateAchievementProgress>();

	selfRef->SteamSubsystem = static_cast<FOnlineSubsystemSteam*>(IOnlineSubsystem::Get());

	selfRef->achievementAPIName = AchievementAPIName;
	selfRef->currentProgress = CurrentProgress;
	selfRef->maxProgress = MaxProgress;

	return selfRef;
}

void UIndicateAchievementProgress::Activate()
{
	if (SteamAPI_Init())
	{
		if (!SteamUserStats()->IndicateAchievementProgress(TCHAR_TO_ANSI(*achievementAPIName), (uint32)currentProgress, (uint32)maxProgress))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to indacate achievement progress!"));

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

void UIndicateAchievementProgress::OnUserStatsStored(UserStatsStored_t* pCallback)
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

void UIndicateAchievementProgress::OnUserAchievementsStored(UserAchievementStored_t* pCallback)
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