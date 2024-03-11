// Copyright Epic Games, Inc. All Rights Reserved.

#include "TarganGameMode.h"
#include "TarganCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATarganGameMode::ATarganGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
