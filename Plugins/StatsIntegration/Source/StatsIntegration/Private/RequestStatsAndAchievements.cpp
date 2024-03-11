// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "RequestStatsAndAchievements.h"
#include "Engine.h"
#include <string>

URequestStatsAndAchievements* URequestStatsAndAchievements::RequestUserStatsAndAchievements()
{
	URequestStatsAndAchievements* selfRef = NewObject<URequestStatsAndAchievements>();

	selfRef->SteamSubsystem = static_cast<FOnlineSubsystemSteam*>(IOnlineSubsystem::Get());

	return selfRef;
}

void URequestStatsAndAchievements::Activate()
{
	if (SteamAPI_Init())
	{
		RequestUserStats();
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to initialize the Steam API!"));

		onFailure.Broadcast(EResult::k_EResultFail);
		RemoveFromRoot();
	}
}

void URequestStatsAndAchievements::RequestUserStats()
{
	SteamAPICall_t hSteamAPICall = SteamUserStats()->RequestUserStats(SteamUser()->GetSteamID());
	m_callRequestUserStats.Set(hSteamAPICall, this, &URequestStatsAndAchievements::OnUserStatsReceived);
}

void URequestStatsAndAchievements::OnUserStatsReceived(UserStatsReceived_t* pCallback, bool bIOFailure)
{
	SteamSubsystem->ExecuteNextTick([pCallback, bIOFailure, this] {
		if (!bIOFailure && pCallback->m_nGameID == SteamUtils()->GetAppID() && pCallback->m_eResult == EResult::k_EResultOK)
		{
			RequestGlobalAchievementPercentages();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to receive stats!"));

			onFailure.Broadcast(pCallback->m_eResult);
			RemoveFromRoot();
		}
	});
}

void URequestStatsAndAchievements::RequestGlobalAchievementPercentages()
{
	SteamAPICall_t hSteamAPICall = SteamUserStats()->RequestGlobalAchievementPercentages();
	m_callRequestGlobalAchievementPercentages.Set(hSteamAPICall, this, &URequestStatsAndAchievements::OnRequestGlobalAchievementPercentages);
}

void URequestStatsAndAchievements::OnRequestGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t* pCallback, bool bIOFailure)
{
	SteamSubsystem->ExecuteNextTick([pCallback, bIOFailure, this] 
	{
		if (!bIOFailure && pCallback->m_nGameID == SteamUtils()->GetAppID())
		{
			if (pCallback->m_eResult == EResult::k_EResultFail)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No global achievement percentages for the current AppId!"));
			}

			onSuccess.Broadcast(EResult::k_EResultOK);
			RemoveFromRoot();
		}
		else
		{
			onFailure.Broadcast(pCallback->m_eResult);
			RemoveFromRoot();
		}
	});
}
