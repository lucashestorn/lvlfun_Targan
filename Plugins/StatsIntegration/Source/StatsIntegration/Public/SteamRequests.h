// Copyright 2021 Artur 'NijoMeisteR' Dworaczek. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamRequests.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType, Category = "Simple Steam Stats & Achievements")
enum class ESteamStatType : uint8
{
	STAT_INT      UMETA(DisplayName = "Integer"),
	STAT_FLOAT    UMETA(DisplayName = "Float"),
	STAT_AVGRATE  UMETA(DisplayName = "Average"),
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType, Category = "Simple Steam Stats & Achievements")
struct FSteamStat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Simple Steam Stats & Achievements")
		FString FriendlyStatName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Simple Steam Stats & Achievements")
		FString APIStatName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Simple Steam Stats & Achievements")
		ESteamStatType StatType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Simple Steam Stats & Achievements")
		int32 IntegerValue;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Simple Steam Stats & Achievements")
		float FloatValue;

	// Constructor
	FSteamStat()
	{
		FriendlyStatName = "";
		APIStatName = "";
		StatType = ESteamStatType::STAT_INT;
		IntegerValue = 0;
		FloatValue = 0.f;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class STATSINTEGRATION_API USteamRequests : public UObject
{
	GENERATED_BODY()
	
};
