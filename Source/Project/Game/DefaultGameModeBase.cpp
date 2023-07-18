// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameModeBase.h"
#include "Actors/DefaultCharacter.h"
#include "UI/DefaultHUD.h"
//#include "UObject/ConstructorHelpers.h"

ADefaultGameModeBase::ADefaultGameModeBase()
{
	/*static ConstructorHelpers::FClassFinder<APawn> DefaultCharacterFinder(TEXT("/Game/Characters/BP_FirstPersonCharacter.BP_FirstPersonCharacter_C"));
	if (DefaultCharacterFinder.Succeeded()) DefaultPawnClass = DefaultCharacterFinder.Class;*/

	DefaultPawnClass = ADefaultCharacter::StaticClass();
	HUDClass = ADefaultHUD::StaticClass();
}
