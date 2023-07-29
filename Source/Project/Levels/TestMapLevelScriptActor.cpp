// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/TestMapLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/SimulatedActor.h"
#include "Actors/TransitionActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OverlayUserWidget.h"
#include "TestMapLevelScriptActor.h"

#define GRAVITY_SWITCH_ID "ButtonActor_1"
#define TEST_BOX_ID "SimulatedActor_1"
#define TEST_CUBE_ID "SimulatedActor_2"

ATestMapLevelScriptActor::ATestMapLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestMapLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check that all objects and containers exist
	if (TestCube && TestBox)
	{
		// Check that all objects are inside their respective containers
		if (IsObjectInContainer(TestCube, TestBox))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Success!"));

			// Dereference objects so this function doesn't get checked anymore
			TestCube = nullptr;
			TestBox = nullptr;

			FVector SpawnLocation = FVector(500, 0, 100);
			FRotator SpawnRotation = FRotator(0);

			TransitionActor = GetWorld()->SpawnActor<ATransitionActor>(SpawnLocation, SpawnRotation);
			
			// TODO:
			// Once all objects are in the container:
			// Replace container with a new "completed" mesh
			// Set a level specific boolean to true as one of the "checks" for finishing the level
			// Set all pointers = nullptr so the function doesnt get checked anymore
		}
	}
}

void ATestMapLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Find all unique actors within the level
	FindSimulatedActors(SimulatedActorArray);
	FindButtonActors(ButtonActorArray);

	// Set GravitySwitch to active
	if (GravitySwitch)
	{
		GravitySwitch->bIsActive = true;
		GravitySwitch->FlipSwitch(GravitySwitch->bIsActive);
	}
}

void ATestMapLevelScriptActor::ButtonTriggered(AButtonActor* const Button)
{
	if (Button == GravitySwitch)
	{
		Button->bIsActive = !Button->bIsActive;
		Button->FlipSwitch(Button->bIsActive);
		SetGravity(Button->bIsActive);
	}
}

void ATestMapLevelScriptActor::FindButtonActors(const TArray<AButtonActor*>& ButtonActors)
{
	for (AButtonActor* ButtonActor : ButtonActorArray)
	{
		if (ButtonActor->GetName() == GRAVITY_SWITCH_ID)
		{
			GravitySwitch = ButtonActor;
		}
	}
}

void ATestMapLevelScriptActor::FindSimulatedActors(const TArray<ASimulatedActor*>& SimulatedActors)
{
	for (ASimulatedActor* SimulatedActor : SimulatedActorArray)
	{
		if (SimulatedActor->GetName() == TEST_CUBE_ID)
		{
			TestCube = SimulatedActor;
		}

		if (SimulatedActor->GetName() == TEST_BOX_ID)
		{
			TestBox = SimulatedActor;
		}
	}
}
