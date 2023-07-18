// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/TestMapLevelScriptActor.h"
#include "Actors/PhysicsStaticMeshActor.h"

ATestMapLevelScriptActor::ATestMapLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestMapLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TestCube && TestBox)
	{
		if (IsObjectInContainer(TestCube, TestBox)) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Success!"));
	}
}

void ATestMapLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Find key items in the level by their ID Name and assign them to references
	for (APhysicsStaticMeshActor* PhysicsActor : PhysicsActorArray)
	{
		if (PhysicsActor->GetName() == "StaticMeshActor_0") TestCube = PhysicsActor;
		if (PhysicsActor->GetName() == "StaticMeshActor_10") TestBox = PhysicsActor;
	}
}
