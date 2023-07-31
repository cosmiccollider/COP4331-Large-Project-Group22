// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameModeBase.h"
#include "Actors/DefaultCharacter.h"

ADefaultGameModeBase::ADefaultGameModeBase()
{
	DefaultPawnClass = ADefaultCharacter::StaticClass();
}
