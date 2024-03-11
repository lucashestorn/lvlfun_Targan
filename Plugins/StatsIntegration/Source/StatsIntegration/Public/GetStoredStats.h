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
#include "GetStoredStats.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API UGetStoredStats : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/* Gets the current value of the a stat for the specified user. You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static void GetUserStoredStats(TArray<FSteamStat> StatsToGet, TArray<FSteamStat>& StatsOut);

	/* Gets the current value of the a stat for the specified user. You must have called RequestUserStats and it needs to return successfully via its callback prior to calling this. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static void GetUserStoredStat(FString APIStatName, ESteamStatType StatType, int32& IntegerValue, float& FloatValue);
};
