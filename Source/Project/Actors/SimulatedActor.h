// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DefaultActor.h"
#include "SimulatedActor.generated.h"

/**
 * SimulatedActor is an Actor that automatically has Simulated Physics enabled and contains physics related functions.
 */
UCLASS()
class PROJECT_API ASimulatedActor : public ADefaultActor
{
	GENERATED_BODY()
	
public:
	ASimulatedActor();

	/**
	 * Sets whether or not this actor is affected by gravity
	 * 
	 * @param	bEnabled	specify true to enable gravity
	 */
	UFUNCTION()
	void SetGravity(const bool bEnabled);
};
