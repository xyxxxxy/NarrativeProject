// Copyright Epic Games, Inc. All Rights Reserved.

#include "NarrativeProjectGameMode.h"
#include "NarrativeProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANarrativeProjectGameMode::ANarrativeProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
