// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/TestMapLevelScriptActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/PhysicsStaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

#define TEST_CUBE_ID "StaticMeshActor_0"
#define TEST_BOX_ID "StaticMeshActor_10"

ATestMapLevelScriptActor::ATestMapLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ATestMapLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TestCube && TestBox)
	{
		if (IsObjectInContainer(TestCube, TestBox))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Success!"));
		}
	}
}

// Called when the game starts or when spawned
void ATestMapLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Find key items in the level by their ID Name and assign them to references
	for (APhysicsStaticMeshActor* PhysicsActor : PhysicsActorArray)
	{
		if (PhysicsActor->GetName() == TEST_CUBE_ID)
		{
			TestCube = PhysicsActor;
		}

		if (PhysicsActor->GetName() == TEST_BOX_ID)
		{
			TestBox = PhysicsActor;
		}
	}
}
