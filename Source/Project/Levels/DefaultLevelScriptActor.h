// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DefaultLevelScriptActor.generated.h"

class AActor;
class AButtonActor;
class ASimulatedActor;
class ATransitionActor;

UENUM()
enum class ENextLevel : uint8
{
	Default,
	Secret
};

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
	/** Array of all Actors in the current level */
	UPROPERTY()
	TArray<AActor*> ActorArray;

	/** Array of all ButtonActors in the current level */
	UPROPERTY()
	TArray<AButtonActor*> ButtonActorArray;

	/** Array of all SimulatedActors in the current level */
	UPROPERTY()
	TArray<ASimulatedActor*> SimulatedActorArray;

	/** Stores the TransitionActor for the current level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATransitionActor* TransitionActor;

	/**
	 * Called to start the ending process for the current level upon clicking an actor
	 *
	 * @param	Actor		specifies the TransitionActor that ends the level
	 * @param	Level		specifies if the next level should be the default level or a special level
	 */
	UFUNCTION()
	void EndLevel(ATransitionActor* const Actor, const ENextLevel Level = ENextLevel::Default);
	
	/**
	 * Called to determine whether an object is inside of a container
	 * 
	 * @param	Object			specifies the object which goes inside the container
	 * @param	Container		specifies the container which holds the object
	 */
	UFUNCTION()
	bool IsObjectInContainer(ASimulatedActor* const Object, ASimulatedActor* const Container);

	/**
	 * Called to open the next level
	 *
	 * @param	Level		specifies if the next level should be the default level or a special level
	 */
	UFUNCTION()
	void NextLevel(const ENextLevel Level = ENextLevel::Default);

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
