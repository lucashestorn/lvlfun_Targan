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
#include "RequestStatsAndAchievements.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseRequestStatsAndAchievementsDelegate, int32, SteamErrorOutput);

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API URequestStatsAndAchievements : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
		FResponseRequestStatsAndAchievementsDelegate onSuccess;

	UPROPERTY(BlueprintAssignable)
		FResponseRequestStatsAndAchievementsDelegate onFailure;
	
	/* Asynchronously downloads stats and achievements from the server. */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Simple Steam Stats & Achievements")
		static URequestStatsAndAchievements* RequestUserStatsAndAchievements();

private:
	FOnlineSubsystemSteam* SteamSubsystem;

	virtual void Activate() override;

	void RequestUserStats();
	void OnUserStatsReceived(UserStatsReceived_t* pCallback, bool bIOFailure);
	CCallResult<URequestStatsAndAchievements, UserStatsReceived_t> m_callRequestUserStats;

	void RequestGlobalAchievementPercentages();
	void OnRequestGlobalAchievementPercentages(GlobalAchievementPercentagesReady_t* pCallback, bool bIOFailure);
	CCallResult<URequestStatsAndAchievements, GlobalAchievementPercentagesReady_t> m_callRequestGlobalAchievementPercentages;
};
