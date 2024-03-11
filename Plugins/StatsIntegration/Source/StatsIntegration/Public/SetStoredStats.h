// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4265)
#pragma warning(disable:4701)

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SteamRequests.h"
#pragma push_macro("ARRAY_COUNT")
#undef ARRAY_COUNT

#if USING_CODE_ANALYSIS
MSVC_PRAGMA(warning(push))
MSVC_PRAGMA(warning(disable : ALL_CODE_ANALYSIS_WARNINGS))
#endif

#include "OnlineSubsystemSteam.h"
#include <steam/steam_api.h>

#if USING_CODE_ANALYSIS
MSVC_PRAGMA(warning(pop))
#endif

#pragma pop_macro("ARRAY_COUNT")
#include "SetStoredStats.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API USetStoredStats : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call 'StoreUserStatsAndAchievements'. 'SessionLength' (seconds since last call to the set stat function) variable only required to calculate averages. */
	UFUNCTION(BlueprintCallable, Category = "Simple Steam Stats & Achievements")
		static bool SetStoredStats(TArray<FSteamStat> SteamStatsIn, float SessionLength);

	/* This call only modifies Steam's in-memory state and is very cheap. Doing so allows Steam to persist the changes even in the event of a game crash or unexpected shutdown. To submit the stats to the server you must call 'StoreUserStatsAndAchievements'. 'SessionLength' (seconds since last call to the set stat function) variable only required to calculate averages. */
	UFUNCTION(BlueprintCallable, Category = "Simple Steam Stats & Achievements")
		static bool SetStoredStat(FString APIStatName, ESteamStatType StatType, float FloatValue, int32 IntegerValue, float SessionLength);
};
