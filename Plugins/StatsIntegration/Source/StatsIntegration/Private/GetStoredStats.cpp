// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#include "GetStoredStats.h"
#include "Engine.h"

void UGetStoredStats::GetUserStoredStats(TArray<FSteamStat> StatsToGet, TArray<FSteamStat>& StatsOut)
{
	if (SteamAPI_Init())
	{
		for (int32 StatI = 0; StatI < StatsToGet.Num(); StatI++)
		{
			FSteamStat stat = StatsToGet[StatI];
			char* statName = TCHAR_TO_ANSI(*stat.APIStatName);

			switch (stat.StatType)
			{
				case ESteamStatType::STAT_INT:
					SteamUserStats()->GetStat(statName, &stat.IntegerValue);
					break;
				case ESteamStatType::STAT_FLOAT:
				case ESteamStatType::STAT_AVGRATE:
					SteamUserStats()->GetStat(statName, &stat.FloatValue);
					break;
			}

			StatsOut.Add(stat);
		}
	}

	return;
}

void UGetStoredStats::GetUserStoredStat(FString APIStatName, ESteamStatType StatType, int32& IntegerValue, float& FloatValue)
{
	if (SteamAPI_Init())
	{
		char* statName = TCHAR_TO_ANSI(*APIStatName);

		switch (StatType)
		{
			case ESteamStatType::STAT_INT:
				SteamUserStats()->GetStat(statName, &IntegerValue);
				break;
			case ESteamStatType::STAT_FLOAT:
			case ESteamStatType::STAT_AVGRATE:
				SteamUserStats()->GetStat(statName, &FloatValue);
				break;
		}
	}

	return;
}