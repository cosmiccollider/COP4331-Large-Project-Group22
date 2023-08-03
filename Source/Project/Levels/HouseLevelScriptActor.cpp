// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/HouseLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/CatActor.h"
#include "Actors/SafeActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/MemoryGameActor.h"
#include "Actors/SimulatedActor.h"
#include "Actors/TransitionActor.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OverlayUserWidget.h"
#include "Components/EditableTextBox.h"

#define GRAVITY_ID "ButtonActor_1"
#define SAFE_ID "SafeActor_1"

#define INSIDE_CONTAINER_ID "StaticMeshActor_427"
#define INSIDE_OBJECT_1_ID "StaticMeshActor_34"
#define INSIDE_OBJECT_2_ID "StaticMeshActor_22"
#define INSIDE_OBJECT_3_ID "StaticMeshActor_376"
#define INSIDE_OBJECT_4_ID "StaticMeshActor_371"
#define INSIDE_OBJECT_5_ID "StaticMeshActor_29"
#define INSIDE_OBJECT_6_ID "StaticMeshActor_32"

#define OUTSIDE_CONTAINER_ID "StaticMeshActor_418"
#define OUTSIDE_OBJECT_1_ID "StaticMeshActor_405"
#define OUTSIDE_OBJECT_2_ID "StaticMeshActor_403"
#define OUTSIDE_OBJECT_3_ID "StaticMeshActor_462"
#define OUTSIDE_OBJECT_4_ID "StaticMeshActor_422"

#define SCUBA_GEAR_ID "StaticMeshActor_12"
#define LEINECKERS_YARD_ID "StaticMeshActor_14"

#define NARRATIVE_DURATION 5.0f

AHouseLevelScriptActor::AHouseLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHouseLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if the initial narrative has been delivered yet
	if (!bInitialNarrativeDelivered)
	{
		DeliverInitialNarrative();
	}

	// Check if the objectives have been delivered yet
	if (!bObjectivesDelivered && bBaristaNarrativeDelivered)
	{
		// Determine if the player has left the bedroom yet
		if (PlayerCharacter && PlayerCharacter->GetActorLocation().X < 1800 && PlayerCharacter->GetActorLocation().Y > 200)
		{
			DeliverObjectives();
		}
	}

	// Cat Easter Egg
	if (!bCheckedCat && bObjectivesDelivered)
	{
		FHitResult OutHit;

		// Check that the actor in front of the player is the cat
		if (PlayerCharacter->CanLineTrace(OutHit) && OutHit.GetActor()->GetClass() == ACatActor::StaticClass())
		{
			if (PlayerOverlay)
			{
				// Tell the player not to disturb the cat
				PlayerOverlay->RevealNarrative();
				PlayerOverlay->SetNarrativeText(FText(FText::FromString("Best not to wake up the kitty.")));

				// Create a timer to remove the narrative after awhile
				float NarrativeDuration = NARRATIVE_DURATION;
				FTimerHandle NarrativeTimer;
				GetWorld()->GetTimerManager().SetTimer(NarrativeTimer, this, &AHouseLevelScriptActor::RemoveNarrative, NarrativeDuration, false);

				bCheckedCat = true;
			}
		}
	}

	// If the memory game isn't unlocked yet, attempt to unlock it
	if (!bMemoryGameUnlocked)
	{
		UnlockMemoryGame();
	}

	// Check if there's a memory game currently being triggered
	// If there is, update the safe correct combination with the memory game correct pattern
	// Increment the values by 1 to be more intuitive than starting at 0
	if (Safe && MemoryGameActor && MemoryGameActor->bActiveAnimation)
	{
		TArray<uint8> Combination = MemoryGameActor->GetCorrectPattern();

		for (uint8 i = 0; i < Combination.Num(); i++)
		{
			Combination[i]++;
		}

		Safe->SetCorrectCombination(Combination);
	}

	if (!bScubaGearReturned)
	{
		if (IsObjectInContainer(ScubaGear, LeineckersYard))
		{
			if (PlayerOverlay)
			{
				PlayerOverlay->RemoveSafeTracker();

				// Deliver the final narrative
				PlayerOverlay->RevealNarrative();
				PlayerOverlay->SetNarrativeText(FText(FText::FromString("He'll never know it was me.")));

				// Create a timer to remove the narrative after awhile
				float NarrativeDuration = NARRATIVE_DURATION;
				FTimerHandle NarrativeTimer;
				GetWorld()->GetTimerManager().SetTimer(NarrativeTimer, this, &AHouseLevelScriptActor::RemoveNarrative, NarrativeDuration, false);
			}

			bScubaGearReturned = true;

			// Spawn the TransitionActor
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor not found: MemoryGameActor"));
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

void AHouseLevelScriptActor::DeliverInitialNarrative()
{
	if (PlayerOverlay)
	{
		// Create a timer to deliver barista narrative
		float NarrativeDuration = PlayerOverlay->TransitionAnimation->GetEndTime() + NARRATIVE_DURATION;
		FTimerHandle NarrativeTimer;
		GetWorld()->GetTimerManager().SetTimer(NarrativeTimer, this, &AHouseLevelScriptActor::DeliverBaristaNarrative, NarrativeDuration, false);

		bInitialNarrativeDelivered = true;
	}
}

void AHouseLevelScriptActor::DeliverBaristaNarrative()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->SetNarrativeText(FText::FromString("Better not be late to my barista job."));
		bBaristaNarrativeDelivered = true;
	}
}

void AHouseLevelScriptActor::DeliverObjectives()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->SetNarrativeText(FText(FText::FromString("Wait... why is the house such a mess?")));

		// Create a timer to reveal the objectives
		float NarrativeDuration = NARRATIVE_DURATION;
		FTimerHandle NarrativeTimer;
		GetWorld()->GetTimerManager().SetTimer(NarrativeTimer, this, &AHouseLevelScriptActor::RevealObjectives, NarrativeDuration, false);

		bObjectivesDelivered = true;
	}
}

void AHouseLevelScriptActor::RevealObjectives()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->RemoveNarrative();
		PlayerOverlay->RevealInsideTracker();
		PlayerOverlay->UpdateInsideTracker(0, InsideObjectsMax);
		PlayerOverlay->RevealOutsideTracker();
		PlayerOverlay->UpdateOutsideTracker(0, OutsideObjectsMax);
	}
}

void AHouseLevelScriptActor::RemoveNarrative()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->RemoveNarrative();
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

	if (!GravitySwitch)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor not found: GravitySwitch"));
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

	if (!Safe)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor not found: Safe"));
	}
}

void AHouseLevelScriptActor::FindSimulatedActors(const TArray<ASimulatedActor*>& SimulatedActors)
{
	for (ASimulatedActor* SimulatedActor : SimulatedActorArray)
	{
		FString ActorName = SimulatedActor->GetName();

		if (ActorName == INSIDE_CONTAINER_ID)
		{
			InsideContainer = SimulatedActor;
		}
		else if (ActorName == OUTSIDE_CONTAINER_ID)
		{
			OutsideContainer = SimulatedActor;
		}
		else if (ActorName == INSIDE_OBJECT_1_ID || ActorName == INSIDE_OBJECT_2_ID
			|| ActorName == INSIDE_OBJECT_3_ID || ActorName == INSIDE_OBJECT_4_ID
			|| ActorName == INSIDE_OBJECT_5_ID || ActorName == INSIDE_OBJECT_6_ID)
		{
			InsideObjects.Add(SimulatedActor);
		}
		else if (ActorName == OUTSIDE_OBJECT_1_ID || ActorName == OUTSIDE_OBJECT_2_ID
			|| ActorName == OUTSIDE_OBJECT_3_ID || ActorName == OUTSIDE_OBJECT_4_ID)
		{
			OutsideObjects.Add(SimulatedActor);
		}
		else if (ActorName == SCUBA_GEAR_ID)
		{
			ScubaGear = SimulatedActor;
		}
		else if (ActorName == LEINECKERS_YARD_ID)
		{
			LeineckersYard = SimulatedActor;
		}
	}

	if (!InsideContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor not found: InsideContainer"));
	}

	if (!OutsideContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor not found: OutsideContainer"));
	}

	if (!ScubaGear)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor not found: ScubaGear"));
	}

	if (!LeineckersYard)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor not found: LeineckersYard"));
	}

	// Set original count as max values for UI purposes
	InsideObjectsMax = InsideObjects.Num();
	OutsideObjectsMax = OutsideObjects.Num();
}

void AHouseLevelScriptActor::CheckObjectsInContainers(TArray<ASimulatedActor*>& Objects, ASimulatedActor* const Container)
{
	for (ASimulatedActor* Object : Objects)
	{
		if (IsObjectInContainer(Object, Container))
		{
			// Remove the object from the array and destroy it
			Objects.Remove(Object);
			Object->Destroy();

			// Update the trackers to reflect the new number of objects
			if (Container == InsideContainer && PlayerOverlay)
			{
				PlayerOverlay->UpdateInsideTracker(InsideObjectsMax - Objects.Num(), InsideObjectsMax);
			}
			else if (Container == OutsideContainer && PlayerCharacter && PlayerOverlay)
			{
				PlayerOverlay->UpdateOutsideTracker(OutsideObjectsMax - Objects.Num(), OutsideObjectsMax);
			}
		}
	}
}

void AHouseLevelScriptActor::UnlockMemoryGame()
{
	// Check if the house is cleaned
	if (!bCleanHouse)
	{
		CheckObjectsInContainers(InsideObjects, InsideContainer);

		if (InsideObjects.Num() == 0)
		{
			bCleanHouse = true;

			if (PlayerOverlay)
			{
				PlayerOverlay->RemoveInsideTracker();
			}
		}
	}

	// Check if the yard is cleaned
	if (!bCleanYard)
	{
		CheckObjectsInContainers(OutsideObjects, OutsideContainer);

		if (OutsideObjects.Num() == 0)
		{
			bCleanYard = true;

			if (PlayerOverlay)
			{
				PlayerOverlay->RemoveOutsideTracker();
			}
		}
	}

	// If both are cleaned, unlock the memory game
	if (bCleanHouse && bCleanYard)
	{
		bMemoryGameUnlocked = true;

		if (PlayerOverlay)
		{
			PlayerOverlay->RevealSafeTracker();
		}
	}
}
