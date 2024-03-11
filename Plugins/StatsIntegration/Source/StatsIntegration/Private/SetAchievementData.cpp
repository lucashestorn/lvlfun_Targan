// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "SetAchievementData.h"

bool USetAchievementData::SetAchievement(FString AchievementAPIName)
{
	if (SteamAPI_Init())
	{
		return SteamUserStats()->SetAchievement(TCHAR_TO_ANSI(*AchievementAPIName));
	}
	
	return false;
}

bool USetAchievementData::ClearAchievement(FString AchievementAPIName)
{
	if (SteamAPI_Init())
	{
		return SteamUserStats()->ClearAchievement(TCHAR_TO_ANSI(*AchievementAPIName));
	}
	
	return false;
}