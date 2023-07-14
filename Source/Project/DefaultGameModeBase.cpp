// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameModeBase.h"
#include "FirstPersonCharacter.h"
#include "DefaultHUD.h"
#include "UObject/ConstructorHelpers.h"

ADefaultGameModeBase::ADefaultGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> FirstPersonCharacterFinder(TEXT("/Game/Characters/BP_FirstPersonCharacter.BP_FirstPersonCharacter_C"));
	if (FirstPersonCharacterFinder.Succeeded()) DefaultPawnClass = FirstPersonCharacterFinder.Class;

	HUDClass = ADefaultHUD::StaticClass();
}
