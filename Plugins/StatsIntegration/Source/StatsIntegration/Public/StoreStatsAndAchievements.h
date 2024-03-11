// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once
#pragma warning(disable:4996)
#pragma warning(disable:4265)
#pragma warning(disable:4701)

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
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
#include "StoreStatsAndAchievements.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseStoreStatsAndAchievementsDelegate, int32, SteamErrorOutput);

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API UStoreStatsAndAchievements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FResponseStoreStatsAndAchievementsDelegate onSuccess;

	UPROPERTY(BlueprintAssignable)
		FResponseStoreStatsAndAchievementsDelegate onFailure;

	/* Asynchronously uploads stats and achievements to the server. */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Simple Steam Stats & Achievements")
		static UStoreStatsAndAchievements* StoreUserStatsAndAchievements();

private:
	FOnlineSubsystemSteam* SteamSubsystem;
	bool bFetchedStatsOrAchievements;

	virtual void Activate() override;
	void UnregisterAllCallbacks();

	STEAM_CALLBACK_MANUAL(UStoreStatsAndAchievements, OnUserStatsStored, UserStatsStored_t, StoreUserStats);
	STEAM_CALLBACK_MANUAL(UStoreStatsAndAchievements, OnUserAchievementsStored, UserAchievementStored_t, StoreUserAchievements);
};
