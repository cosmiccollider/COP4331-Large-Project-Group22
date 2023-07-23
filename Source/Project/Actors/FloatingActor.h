// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DefaultActor.h"
#include "FloatingActor.generated.h"

/**
 * FloatingActor is an Actor that has the ability to float up and down as well as rotate.
 */
UCLASS()
class PROJECT_API AFloatingActor : public ADefaultActor
{
	GENERATED_BODY()

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Specifies the speed at which the actor floats up and down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
	float FloatSpeed = 20.f;

	/** Specifies the speed at which the actor rotates in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
	float RotationSpeed = 20.f;
};
