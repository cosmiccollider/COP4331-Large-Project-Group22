// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/DefaultLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/DefaultActor.h"
#include "Actors/DefaultCharacter.h"
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

	// Get all DefaultActors in the level and sort them into subclass arrays
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADefaultActor::StaticClass(), ActorArray);
	SortActors(ActorArray);
}

bool ADefaultLevelScriptActor::IsObjectInContainer(ASimulatedActor* const Object, ASimulatedActor* const Container)
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

void ADefaultLevelScriptActor::SortActors(const TArray<AActor*>& Actors)
{
	for (AActor* Actor : ActorArray)
	{
		// Cast each Actor to each subclass of DefaultActor
		// If the cast fails, the type doesn't match so it checks the next type
		// If the cast succeeds, we've found the correct type so we add it to the subclass array
		if (AButtonActor* ButtonActor = Cast<AButtonActor>(Actor))
		{
			ButtonActorArray.Add(ButtonActor);
		}
		else if (ASimulatedActor* SimulatedActor = Cast<ASimulatedActor>(Actor))
		{
			SimulatedActorArray.Add(SimulatedActor);
		}
	}
}
