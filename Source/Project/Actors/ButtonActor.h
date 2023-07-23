// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DefaultActor.h"
#include "ButtonActor.generated.h"

/**
 * ButtonActor is an interactable Actor that allows the player to trigger events within the level.
 */
UCLASS()
class PROJECT_API AButtonActor : public ADefaultActor
{
	GENERATED_BODY()
	
public:
	/** Determines whether a switch is currently on or off */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActive;

	/**
	 * Rotates the static mesh component of a switch to an on or off position
	 * 
	 * @param	bActive		specify true to flip the switch into the "on" position
	 */
	UFUNCTION()
	void FlipSwitch(const bool bActive);
};
