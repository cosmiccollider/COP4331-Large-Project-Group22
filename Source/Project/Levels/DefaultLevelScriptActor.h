// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DefaultLevelScriptActor.generated.h"

class AActor;
class AButtonActor;
class ASimulatedActor;

/**
 * DefaultLevelScriptActor provides a base class derived from LevelScriptActor which we can make specific level scripts from.
 */
UCLASS()
class PROJECT_API ADefaultLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ADefaultLevelScriptActor();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Array of all Actors in the level */
	UPROPERTY()
	TArray<AActor*> ActorArray;

	/** Array of all ButtonActors in the level */
	UPROPERTY()
	TArray<AButtonActor*> ButtonActorArray;

	/** Array of all SimulatedActors in the level */
	UPROPERTY()
	TArray<ASimulatedActor*> SimulatedActorArray;
	
	/**
	 * Called to determine whether an object is inside of a container
	 * 
	 * @param	Object			specifies the object which goes inside the container
	 * @param	Container		specifies the contianer which holds the object
	 */
	UFUNCTION()
	bool IsObjectInContainer(ASimulatedActor* const Object, ASimulatedActor* const Container);

	/**
	 * Called to set whether or not the player character and all SimulatedActors in the level are affected by gravity
	 *
	 * @param	bEnabled	specify true to enable gravity
	 */
	UFUNCTION()
	void SetGravity(const bool bEnabled);

	/**
	 * Called to sort all Actors into their respective subclass arrays
	 *
	 * @param	Actors		specifies the array of all Actors
	 */
	UFUNCTION()
	void SortActors(const TArray<AActor*>& Actors);
};
