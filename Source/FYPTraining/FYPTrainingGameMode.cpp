// Copyright Epic Games, Inc. All Rights Reserved.

#include "FYPTrainingGameMode.h"
#include "FYPTrainingCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFYPTrainingGameMode::AFYPTrainingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_RTS_Camera"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
