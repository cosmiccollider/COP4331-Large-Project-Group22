// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/HouseLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/SafeActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/MemoryGameActor.h"
#include "Actors/SimulatedActor.h"
#include "Actors/TransitionActor.h"

#define GRAVITY_ID "Placeholder"
#define SAFE_ID "Placeholder"

#define INSIDE_CONTAINER_ID "Placeholder"
#define INSIDE_OBJECT_1_ID "Placeholder"
#define INSIDE_OBJECT_2_ID "Placeholder"
#define INSIDE_OBJECT_3_ID "Placeholder"

#define OUTSIDE_CONTAINER_ID "Placeholder"
#define OUTSIDE_OBJECT_1_ID "Placeholder"
#define OUTSIDE_OBJECT_2_ID "Placeholder"
#define OUTSIDE_OBJECT_3_ID "Placeholder"

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

	// If the memory game isn't unlocked yet, continue to check if objects are being put in containers
	if (!bMemoryGameUnlocked)
	{
		// Check if the house is cleaned
		if (!bCleanHouse)
		{
			CheckObjectsInContainers(InsideObjects, InsideContainer);
			
			if (InsideObjects.Num() == 0)
			{
				bCleanHouse = true;
			}
		}

		// Check if the yard is cleaned
		if (!bCleanYard)
		{
			CheckObjectsInContainers(OutsideObjects, OutsideContainer);

			if (OutsideObjects.Num() == 0)
			{
				bCleanYard = true;
			}
		}

		// If both are cleaned, unlock the memory game
		if (bCleanHouse && bCleanYard)
		{
			bMemoryGameUnlocked = true;
		}
	}
	
	/*FVector SpawnLocation = FVector(-180, 400, 250);
	FRotator SpawnRotation = FRotator(0);

	TransitionActor = GetWorld()->SpawnActor<ATransitionActor>(SpawnLocation, SpawnRotation);*/
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

void AHouseLevelScriptActor::MemoryGameTriggered(AMemoryGameActor* const CurrentMemoryGameActor)
{
	if (bMemoryGameUnlocked)
	{
		CurrentMemoryGameActor->MemoryGameTriggered();
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
		if (ButtonActor->GetName() == GRAVITY_ID)
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
		if (SimulatedActor->GetName() == INSIDE_CONTAINER_ID)
		{
			InsideContainer = SimulatedActor;
		}
		else if (SimulatedActor->GetName() == OUTSIDE_CONTAINER_ID)
		{
			OutsideContainer = SimulatedActor;
		}
		else if (SimulatedActor->GetName() == INSIDE_OBJECT_1_ID
		|| SimulatedActor->GetName() == INSIDE_OBJECT_2_ID
		|| SimulatedActor->GetName() == INSIDE_OBJECT_3_ID)
		{
			InsideObjects.Add(SimulatedActor);
		}
		else if (SimulatedActor->GetName() == OUTSIDE_OBJECT_1_ID
		|| SimulatedActor->GetName() == OUTSIDE_OBJECT_2_ID
		|| SimulatedActor->GetName() == OUTSIDE_OBJECT_3_ID)
		{
			OutsideObjects.Add(SimulatedActor);
		}
	}
}

void AHouseLevelScriptActor::CheckObjectsInContainers(TArray<ASimulatedActor*> Objects, ASimulatedActor* Container)
{
	for (ASimulatedActor* Object : Objects)
	{
		if (IsObjectInContainer(Object, Container))
		{
			Objects.Remove(Object);
		}
	}
}
