// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4265)
#pragma warning(disable:4701)

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
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
#include "GetAchievementData.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EAchievementDisplayAttributeUE4 : uint8
{
	ADA_EAttributeName           UMETA(DisplayName = "Name"),
	ADA_EAttributeDescription    UMETA(DisplayName = "Description"),
	ADA_EAttributeHidden         UMETA(DisplayName = "Hidden Status"),
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API UGetAchievementData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* You must have called 'RequestUserStatsAndAchievements' and it needs to return successfully via its callback prior to calling this. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static int32 GetNumAchievements();

	/* You must have called 'RequestUserStatsAndAchievements' and it needs to return successfully via its callback prior to calling this. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static FString GetAchievementAPIName(int32 AchievementIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static UTexture2D* GetAchievementIcon(FString AchievementAPIName);

	/* You must have called 'RequestUserStatsAndAchievements' and it needs to return successfully via its callback prior to calling this. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static void GetAchievementUnlockStatusAndUnlockTime(FString AchievementAPIName, bool& AchievementUnlocked, FDateTime& UnlockTime);

	/* Returns the percentage of users who have unlocked the specified achievement. You must have called 'RequestUserStatsAndAchievements' and it needs to return successfully via its callback prior to calling this. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static float GetGlobalAchievementAchievedPercent(FString AchievementAPIName);

	/* Get general attributes for an achievement. Currently provides: Name, Description, and Hidden status. You must have called 'RequestUserStatsAndAchievements' and it needs to return successfully via its callback prior to calling this. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Simple Steam Stats & Achievements")
		static FString GetAchievementDisplayAttribute(FString AchievementAPIName, EAchievementDisplayAttributeUE4 Attribute);
};
