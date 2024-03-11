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
#include "ResetAllStatsAndAchievements.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseResetStatsAndAchievementsDelegate, int32, SteamErrorOutput);

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API UResetAllStatsAndAchievements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
		FResponseResetStatsAndAchievementsDelegate onSuccess;

	UPROPERTY(BlueprintAssignable)
		FResponseResetStatsAndAchievementsDelegate onFailure;

	UPROPERTY()
		bool bresetAchievementsToo;

	/* Asynchronously uploads stats and achievements to the server. */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Simple Steam Stats & Achievements")
		static UResetAllStatsAndAchievements* ResetUserStatsAndAchievements(/* Reset achievements too? */ bool bAchievementsToo);

private:
	bool bFetchedStatsOrAchievements;
	FOnlineSubsystemSteam* SteamSubsystem;

	virtual void Activate() override;

	void UnregisterAllCallbacks();

	STEAM_CALLBACK_MANUAL(UResetAllStatsAndAchievements, OnUserStatsStored, UserStatsStored_t, UserStatsStored);
	STEAM_CALLBACK_MANUAL(UResetAllStatsAndAchievements, OnUserAchievementsStored, UserAchievementStored_t, UserAchievementsStored);
	STEAM_CALLBACK_MANUAL(UResetAllStatsAndAchievements, OnUserStatsReceived, UserStatsReceived_t, UserStatsReceived);

	void OnRequestGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t* pCallback, bool bIOFailure);
	CCallResult<UResetAllStatsAndAchievements, GlobalAchievementPercentagesReady_t> m_callRequestGlobalAchievementPercentages;
};
