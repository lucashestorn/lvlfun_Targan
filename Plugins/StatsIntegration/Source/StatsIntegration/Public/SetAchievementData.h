// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4265)
#pragma warning(disable:4701)

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
#include "SetAchievementData.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API USetAchievementData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/* Unlocks an achievement. You must have called 'RequestUserStatsAndAchievements' and it needs to return successfully via its callback prior to calling this. To send the unlock status to the server and to trigger the Steam overlay notification you must call 'StoreStatsAndAchievements'. */
	UFUNCTION(BlueprintCallable, Category = "Simple Steam Stats & Achievements")
		static bool SetAchievement(FString AchievementAPIName);

	/* Resets the unlock status of an achievement. You must have called 'RequestUserStatsAndAchievements' and it needs to return successfully via its callback prior to calling this. To send the unlock status to the server and to trigger the Steam overlay notification you must call 'StoreStatsAndAchievements'. */
	UFUNCTION(BlueprintCallable, Category = "Simple Steam Stats & Achievements")
		static bool ClearAchievement(FString AchievementAPIName);
};
