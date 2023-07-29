// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/SimulatedActor.h"
#include "DoorActor.generated.h"

class UPhysicsConstraintComponent;

/**
 * DoorActor represents a door within a level that can swing open using physics
 */
UCLASS()
class PROJECT_API ADoorActor : public ASimulatedActor
{
	GENERATED_BODY()
	
public:
	ADoorActor();

	/** Stores the PhysicsConstraintComponent for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint;
};
