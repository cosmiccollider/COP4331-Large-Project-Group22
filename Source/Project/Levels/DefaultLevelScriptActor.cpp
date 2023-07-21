// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/DefaultLevelScriptActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/PhysicsStaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/OverlayUserWidget.h"

ADefaultLevelScriptActor::ADefaultLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
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

// Called to set gravity for all physics actors in the level
void ADefaultLevelScriptActor::SetGravity(bool bEnabled)
{
	// Set gravity for player
	ADefaultCharacter* Player = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetGravity(bEnabled);

	// Set gravity for objects
	for (APhysicsStaticMeshActor* PhysicsActor : PhysicsActorArray) {
		PhysicsActor->SetGravity(bEnabled);
	}
}
