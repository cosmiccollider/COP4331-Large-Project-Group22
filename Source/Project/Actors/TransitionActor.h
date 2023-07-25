// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DefaultActor.h"
#include "TransitionActor.generated.h"

/**
 * TransitionActor is an Actor that floats and rotates in place, then transitions to the next level when clicked.
 */
UCLASS()
class PROJECT_API ATransitionActor : public ADefaultActor
{
	GENERATED_BODY()
	
public:
	ATransitionActor();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

public:
	/** Specifies the speed at which the actor floats up and down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransitionActor")
		float FloatSpeed = 20.f;

	/** Specifies the speed at which the actor rotates in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransitionActor")
		float RotationSpeed = 20.f;
};
