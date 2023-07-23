// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/TestMapLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/SimulatedActor.h"

#define GRAVITY_SWITCH_ID "ButtonActor_1"
#define TEST_CUBE_ID "PhysicsActor_6"
#define TEST_BOX_ID "PhysicsActor_1"

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
			// TODO:
			// Once all objects are in the container:
			// Replace container with a new "completed" mesh
			// Set a level specific boolean to true as one of the "checks" for finishing the level
			// Set all pointers = nullptr so the function doesnt get called anymore (for performance purposes)
		}
	}
}

void ATestMapLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Find SimulatedActors in the level by their ID Name and assign them to references
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

	// Find ButtonActors in the level by their ID Name and assign them to references
	for (AButtonActor* ButtonActor : ButtonActorArray)
	{
		if (ButtonActor->GetName() == GRAVITY_SWITCH_ID)
		{
			GravitySwitch = ButtonActor;
		}
	}

	// Set GravitySwitch to active
	if (GravitySwitch)
	{
		GravitySwitch->bIsActive = true;
		GravitySwitch->FlipSwitch(GravitySwitch->bIsActive);
	}
}

void ATestMapLevelScriptActor::ButtonPressed(AButtonActor* const Button)
{
	if (Button == GravitySwitch)
	{
		Button->bIsActive = !Button->bIsActive;
		Button->FlipSwitch(Button->bIsActive);
		SetGravity(Button->bIsActive);
	}
}
