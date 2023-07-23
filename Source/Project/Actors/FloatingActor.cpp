// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FloatingActor.h"

void AFloatingActor::Tick(float DeltaTime)
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
