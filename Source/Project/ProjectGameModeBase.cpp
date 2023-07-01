// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectGameModeBase.h"
#include "FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectGameModeBase::AProjectGameModeBase()
	: Super()
{
	// Set default pawn class to our character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Source/Project/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
