// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DefaultActor.h"
#include "MemoryGameActor.generated.h"

class USoundBase;

UENUM()
enum class EClearTimer : uint8
{
	Single,
	All
};

/**
 * MemoryGameActor provides a class of actors from which we can create and play a memory game.
 */
UCLASS()
class PROJECT_API AMemoryGameActor : public ADefaultActor
{
	GENERATED_BODY()
	
protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Called to trigger a MemoryGameActor to either start or continue a game */
	UFUNCTION()
	void MemoryGameTriggered();
	
	/** Determines whether an animation is currently in progress or not */
	bool bActiveAnimation = false;

protected:
	/** Determines whether a game is currently in progress or not */
	bool bGameActive = false;

	/** Stores the index at which a continue event will be triggered */
	int32 ContinueIndex = 0;

	/** Stores the current player patern, until a continue event is reached */
	TArray<int32> PlayerPattern;

	/** Stores the correct pattern required to win the game */
	TArray<int32> CorrectPattern;
	
	/** Array of all Actors in the current level */
	UPROPERTY()
	TArray<AActor*> ActorArray;

	/** Array of all MemoryGameActors in the current level */
	UPROPERTY()
	TArray<AMemoryGameActor*> MemoryGameActorArray;

	/**
	 * Called to begin a new memory game
	 *
	 * @param	Pattern		specifies the correct pattern for the new game
	 */
	UFUNCTION()
	void StartMemoryGame(TArray<int32> Pattern);

	/** Called to assign the current actor to the player pattern */
	UFUNCTION()
	void PlayMemoryGame();

	/** Called to end a game in progress */
	UFUNCTION()
	void EndMemoryGame();

	/** Called to sync the properties of all MemoryGameActors within the level */
	UFUNCTION()
	void SyncMemoryGameActors();

	/** Called to determine if the player pattern and correct pattern are correct so far */
	UFUNCTION()
	bool IsPatternCorrect();

	/** Called to play the selection animation for this actor */
	UFUNCTION()
	void SelectAnimation();
	
	/**
	 * Called to play the continue animation for each actor in the correct pattern
	 *
	 * @param	ActorIndex		specifies the current actor to play the animation for in the recursive loop
	 */
	UFUNCTION()
	void ContinueAnimation(int32 ActorIndex);

	/** Called to play the win animation for all actors */
	UFUNCTION()
	void WinAnimation();

	/** Called to play the lose animation for all actors */
	UFUNCTION()
	void LoseAnimation();

	/**
	 * Called to clear any animations on one or all actors
	 *
	 * @param	Actors		specifies whether we are clearing just one or all of the actors
	 */
	UFUNCTION()
	void ClearAnimation(const EClearTimer Actors);

	/**
	 * Called to lock the player out of input while a game loop is in progress
	 *
	 * @param	bLocked		specify true to lock the player out of input
	 */
	UFUNCTION()
	void LockInput(const bool bLocked);
};
