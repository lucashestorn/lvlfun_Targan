// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4265)
#pragma warning(disable:4701)

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
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
#include "IndicateAchievementProgress.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseIndicateAchievementProgressDelegate, int32, SteamErrorOutput);

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API UIndicateAchievementProgress : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
		FResponseIndicateAchievementProgressDelegate onSuccess;

	UPROPERTY(BlueprintAssignable)
		FResponseIndicateAchievementProgressDelegate onFailure;

	/* Shows the user a pop-up notification with the current progress of an achievement. Calling this function will NOT set the progress or unlock the achievement, the game must do that manually by calling Set Stat / Set Achievement! */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Simple Steam Stats & Achievements")
		static UIndicateAchievementProgress* IndicateUserAchievementProgress(FString AchievementAPIName, int32 CurrentProgress, int32 MaxProgress);

	UPROPERTY()
		FString achievementAPIName;

	UPROPERTY()
		int32 currentProgress;

	UPROPERTY()
		int32 maxProgress;
private:
	bool bFetchedStatsOrAchievements;
	FOnlineSubsystemSteam* SteamSubsystem;

	virtual void Activate() override;

	void UnregisterAllCallbacks();

	STEAM_CALLBACK_MANUAL(UIndicateAchievementProgress, OnUserStatsStored, UserStatsStored_t, UserStatsStored);
	STEAM_CALLBACK_MANUAL(UIndicateAchievementProgress, OnUserAchievementsStored, UserAchievementStored_t, UserAchievementsStored);
};
