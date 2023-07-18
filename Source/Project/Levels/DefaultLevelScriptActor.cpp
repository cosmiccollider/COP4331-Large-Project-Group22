// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultLevelScriptActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/PhysicsStaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ADefaultLevelScriptActor::ADefaultLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ADefaultLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefaultLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Get all physics actors in the level and add them to their own array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APhysicsStaticMeshActor::StaticClass(), ActorArray);

	// Cast all Actors to array of PhysicsActors so we can access functions of that class
	for (AActor* Actor : ActorArray)
	{
		PhysicsActorArray.Add(Cast<APhysicsStaticMeshActor>(Actor));
	}
}

// Called to determine if an object is inside a container
bool ADefaultLevelScriptActor::IsObjectInContainer(APhysicsStaticMeshActor* Object, APhysicsStaticMeshActor* Container)
{
	FVector ObjectOrigin = Object->GetActorLocation();

	FVector ContainerOrigin;
	FVector ContainerBoxExtent;
	Container->GetActorBounds(false, ContainerOrigin, ContainerBoxExtent);

	return UKismetMathLibrary::IsPointInBox(ObjectOrigin, ContainerOrigin, ContainerBoxExtent);
}

// Called to enable gravity for all physics actors in the level
void ADefaultLevelScriptActor::EnableGravity()
{
	// Enable gravity for player
	ADefaultCharacter* Player = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->EnableGravity();

	// Enable gravity for objects
	for (APhysicsStaticMeshActor* PhysicsActor : PhysicsActorArray) {
		PhysicsActor->EnableGravity();
	}
}

// Called to diable gravity for all physics actors in the level
void ADefaultLevelScriptActor::DisableGravity()
{
	// Disable gravity for player
	ADefaultCharacter* Player = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->DisableGravity();

	// Disable gravity for objects
	for (APhysicsStaticMeshActor* PhysicsActor : PhysicsActorArray) {
		PhysicsActor->DisableGravity();
	}
}
