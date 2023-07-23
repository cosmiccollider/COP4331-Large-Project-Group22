// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/DefaultLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/DefaultActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/FloatingActor.h"
#include "Actors/SimulatedActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ADefaultLevelScriptActor::ADefaultLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Get all actors in the level and store them in an array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADefaultActor::StaticClass(), ActorArray);

	// Cast all Actors to their respective class arrays
	for (AActor* Actor : ActorArray)
	{
		if (AButtonActor* ButtonActor = Cast<AButtonActor>(Actor))
		{
			ButtonActorArray.Add(ButtonActor);
		}
		else if (AFloatingActor* FloatingActor = Cast<AFloatingActor>(Actor))
		{
			FloatingActorArray.Add(FloatingActor);
		}
		else if (ASimulatedActor* SimulatedActor = Cast<ASimulatedActor>(Actor))
		{
			SimulatedActorArray.Add(SimulatedActor);
		}
	}
}

bool ADefaultLevelScriptActor::IsObjectInContainer(ASimulatedActor* Object, ASimulatedActor* Container)
{
	FVector ObjectOrigin = Object->GetActorLocation();

	FVector ContainerOrigin;
	FVector ContainerBoxExtent;
	Container->GetActorBounds(false, ContainerOrigin, ContainerBoxExtent);

	return UKismetMathLibrary::IsPointInBox(ObjectOrigin, ContainerOrigin, ContainerBoxExtent);
}

void ADefaultLevelScriptActor::SetGravity(const bool bEnabled)
{
	// Set gravity for player
	ADefaultCharacter* Player = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetGravity(bEnabled);

	// Set gravity for objects
	for (ASimulatedActor* SimulatedActor : SimulatedActorArray) {
		SimulatedActor->SetGravity(bEnabled);
	}
}
