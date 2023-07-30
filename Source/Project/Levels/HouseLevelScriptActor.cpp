// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/HouseLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/SafeActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/MemoryGameActor.h"
#include "Actors/SimulatedActor.h"
#include "Actors/TransitionActor.h"

#define GRAVITY_SWITCH_ID "ButtonActor_1"
#define PLACEHOLDER_CONTAINER_ID "SimulatedActor_1"
#define PLACEHOLDER_OBJECT_ID "SimulatedActor_2"
#define SAFE_ID "SafeActor_1"

AHouseLevelScriptActor::AHouseLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHouseLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if there's a memory game currently active
	// If there is, update the safe correct combination with the memory game correct pattern
	if (MemoryGameActor && MemoryGameActor->bGameActive)
	{
		Safe->SetCorrectCombination(MemoryGameActor->GetCorrectPattern());
	}

	// Check that all objects and containers exist
	if (PlaceholderObject && PlaceholderContainer)
	{
		// Check that all objects are inside their respective containers
		if (IsObjectInContainer(PlaceholderObject, PlaceholderContainer))
		{
			// Dereference objects so this function doesn't get checked anymore
			PlaceholderObject = nullptr;
			PlaceholderContainer = nullptr;

			// Change star location to inside safe (also change scale so it doesn't clip through)
			FVector SpawnLocation = FVector(-180, 400, 250);
			FRotator SpawnRotation = FRotator(0);

			TransitionActor = GetWorld()->SpawnActor<ATransitionActor>(SpawnLocation, SpawnRotation);
		}
	}
}

void AHouseLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Find all unique actors within the level
	FindButtonActors(ButtonActorArray);
	FindSafeActors(SafeActorArray);
	FindSimulatedActors(SimulatedActorArray);

	// Store the first element of the MemoryGameActorArray so we can access its variables
	// We only need the first actor because MemoryGameActors sync their variables between each other
	if (MemoryGameActorArray.Num() > 0)
	{
		MemoryGameActor = MemoryGameActorArray[0];
	}

	// Set GravitySwitch to active
	if (GravitySwitch)
	{
		GravitySwitch->bIsActive = true;
		GravitySwitch->FlipSwitch(GravitySwitch->bIsActive);
	}
}

void AHouseLevelScriptActor::ButtonTriggered(AButtonActor* const CurrentButton)
{
	if (CurrentButton == GravitySwitch)
	{
		CurrentButton->bIsActive = !CurrentButton->bIsActive;
		CurrentButton->FlipSwitch(CurrentButton->bIsActive);
		SetGravity(CurrentButton->bIsActive);
	}
}

void AHouseLevelScriptActor::SafeTriggered(ASafeActor* const CurrentSafe)
{
	if (CurrentSafe == Safe)
	{
		Safe->AttemptSafe();
	}
}

void AHouseLevelScriptActor::FindButtonActors(const TArray<AButtonActor*>& ButtonActors)
{
	for (AButtonActor* ButtonActor : ButtonActorArray)
	{
		if (ButtonActor->GetName() == GRAVITY_SWITCH_ID)
		{
			GravitySwitch = ButtonActor;
		}
	}
}

void AHouseLevelScriptActor::FindSafeActors(const TArray<ASafeActor*>& SafeActors)
{
	for (ASafeActor* SafeActor : SafeActorArray)
	{
		if (SafeActor->GetName() == SAFE_ID)
		{
			Safe = SafeActor;
		}
	}
}

void AHouseLevelScriptActor::FindSimulatedActors(const TArray<ASimulatedActor*>& SimulatedActors)
{
	for (ASimulatedActor* SimulatedActor : SimulatedActorArray)
	{
		if (SimulatedActor->GetName() == PLACEHOLDER_OBJECT_ID)
		{
			PlaceholderObject = SimulatedActor;
		}

		if (SimulatedActor->GetName() == PLACEHOLDER_CONTAINER_ID)
		{
			PlaceholderContainer = SimulatedActor;
		}
	}
}
