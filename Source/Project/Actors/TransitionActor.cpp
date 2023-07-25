// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TransitionActor.h"

ATransitionActor::ATransitionActor()
{
	// Set the default static mesh to a gold star
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StarFinder(TEXT("/Game/Props/Star.Star"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> GoldFinder(TEXT("/Game/Materials/MI_Gold.MI_Gold"));
	if (StarFinder.Succeeded() && GoldFinder.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StarFinder.Object);
		StaticMeshComponent->SetMaterial(0, GoldFinder.Object);
	}
}

void ATransitionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();

	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	float DeltaRotation = DeltaTime * RotationSpeed;

	NewLocation.Z += DeltaHeight * FloatSpeed;
	NewRotation.Yaw += DeltaRotation;

	SetActorLocationAndRotation(NewLocation, NewRotation);
}
