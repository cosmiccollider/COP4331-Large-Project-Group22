// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/DefaultLevelScriptActor.h"
#include "HouseLevelScriptActor.generated.h"

class AButtonActor;
class ASafeActor;
class AMemoryGameActor;
class ASimulatedActor;

/**
 * HouseLevelScriptActor provides the level specific capabilities of the HouseMap.
 */
UCLASS()
class PROJECT_API AHouseLevelScriptActor : public ADefaultLevelScriptActor
{
	GENERATED_BODY()
	
public:
	AHouseLevelScriptActor();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/**
	 * Triggers an event within the level based on the button that was pressed
	 *
	 * @param	Button		specifies the button that was pressed
	 */
	UFUNCTION()
	void ButtonTriggered(AButtonActor* const CurrentButton);

	/**
	 * Triggers the memory game actor to either start or play a game
	 *
	 * @param	MemoryGameActor		specifies the memory game actor that was triggered
	 */
	UFUNCTION()
	void MemoryGameTriggered(AMemoryGameActor* const MemoryGameActor);

	/**
	 * Triggers a safe interaction based on the safe that was clicked
	 *
	 * @param	Safe		specifies the safe that was clicked
	 */
	UFUNCTION()
	void SafeTriggered(ASafeActor* const CurrentSafe);

protected:
	/** Determines whether the house is clean or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCleanHouse = false;

	/** Determines whether the yard is clean or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCleanYard = false;

	/** Determines whether the memory game is unlocked or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMemoryGameUnlocked = false;

	/** Stores the GravitySwitch for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AButtonActor* GravitySwitch;

	/** Stores the MemoryGameActor for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMemoryGameActor* MemoryGameActor;

	/** Stores the Safe for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASafeActor* Safe;

	/** Stores the InsideObjects for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASimulatedActor*> InsideObjects;

	/** Stores the InsideContainer for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* InsideContainer;

	/** Stores the OutsideObjects for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASimulatedActor*> OutsideObjects;

	/** Stores the OutsideContainer for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* OutsideContainer;

	/**
	 * Finds all unique ButtonActors based on defined actor ID names
	 *
	 * @param	ButtonActors		specifies the array of ButtonActors to search
	 */
	UFUNCTION()
	void FindButtonActors(const TArray<AButtonActor*>& ButtonActors);

	/**
	 * Finds all unique SafeActors based on defined actor ID names
	 *
	 * @param	SafeActors		specifies the array of SafeActors to search
	 */
	UFUNCTION()
	void FindSafeActors(const TArray<ASafeActor*>& SafeActors);

	/**
	 * Finds all unique SimulatedActors based on defined actor ID names
	 *
	 * @param	SimulatedActors		specifies the array of SimulatedActors to search
	 */
	UFUNCTION()
	void FindSimulatedActors(const TArray<ASimulatedActor*>& SimulatedActors);

	/** Called to check whether all simulated actors are in their correct containers */
	UFUNCTION()
	void CheckObjectsInContainers(TArray<ASimulatedActor*> Objects, ASimulatedActor* Container);
};
