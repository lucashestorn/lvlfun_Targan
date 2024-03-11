// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "SetStoredStats.h"
#include "Engine.h"

bool USetStoredStats::SetStoredStats(TArray<FSteamStat> SteamStatsIn, float SessionLength)
{
	if (SteamAPI_Init())
	{
		for (int32 StatI = 0; StatI < SteamStatsIn.Num(); StatI++)
		{
			FSteamStat stat = SteamStatsIn[StatI];
			char* statName = TCHAR_TO_ANSI(*stat.APIStatName);

			switch (stat.StatType)
			{
				case ESteamStatType::STAT_INT:
					if (!SteamUserStats()->SetStat(statName, stat.IntegerValue))
						return false;
					break;
				case ESteamStatType::STAT_FLOAT:
					if (!SteamUserStats()->SetStat(statName, stat.FloatValue))
						return false;
					break;
				case ESteamStatType::STAT_AVGRATE:
					if (!SteamUserStats()->UpdateAvgRateStat(statName, stat.FloatValue, (double)SessionLength))
						return false;
					break;
			}
		}
		
		return true;
	}

	return false;
}

bool USetStoredStats::SetStoredStat(FString APIStatName, ESteamStatType StatType, float FloatValue, int32 IntegerValue, float SessionLength)
{
	if (SteamAPI_Init())
	{
		char* statName = TCHAR_TO_ANSI(*APIStatName);

		switch (StatType)
		{
			case ESteamStatType::STAT_INT:
				return SteamUserStats()->SetStat(statName, IntegerValue);
				break;
			case ESteamStatType::STAT_FLOAT:
				return SteamUserStats()->SetStat(statName, FloatValue);
				break;
			case ESteamStatType::STAT_AVGRATE:
				return SteamUserStats()->UpdateAvgRateStat(statName, FloatValue, (double)SessionLength);
				break;
		}
	}

	return false;
}