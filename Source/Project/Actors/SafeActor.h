// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/DefaultActor.h"
#include "SafeActor.generated.h"

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;
class UStaticMeshComponent;

/**
 * SafeActor is a locked safe that only opens when the correct combination is entered into its user interface
 */
UCLASS()
class PROJECT_API ASafeActor : public ADefaultActor
{
	GENERATED_BODY()
	
public:
	ASafeActor();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Called to return the correct combination for this actor */
	UFUNCTION()
	FString GetCorrectCombination() const { return CorrectCombination; };

	/**
	 * Called to set the correct combination for this actor
	 * 
	 * @param	Combination		specifies an array which represents each correct digit in order
	 */
	UFUNCTION()
	void SetCorrectCombination(TArray<uint8> Combination);

	/** Called to open the user interface and attempt to open the safe */
	UFUNCTION()
	void AttemptSafe();

	/** Called to play the animation of opening the safe */
	UFUNCTION()
	void OpenSafe();

protected:
	/** Determines whether the safe is currently open or not */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSafeOpen = false;

	/** Determines the correct combination in string form */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CorrectCombination = "1111";

	/** Stores the DoorStaticMeshComponent for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DoorStaticMeshComponent;

	/** Stores the SafeLevelSequence for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULevelSequence* SafeLevelSequence;

	/** Stores the LevelSequenceActor for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ALevelSequenceActor* LevelSequenceActor;

	/** Stores the LevelSequencePlayer for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ULevelSequencePlayer* LevelSequencePlayer;
};
