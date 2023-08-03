// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/DefaultLevelScriptActor.h"
#include "HouseLevelScriptActor.generated.h"

class AButtonActor;
class ADefaultCharacter;
class AMemoryGameActor;
class ASafeActor;
class ASimulatedActor;
class UOverlayUserWidget;

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
	/** Stores the player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ADefaultCharacter* PlayerCharacter;

	/** Stores the player overlay */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UOverlayUserWidget* PlayerOverlay;

	/** Stores the max number of InsideObjects for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 InsideObjectsMax;

	/** Stores the max number of OutsideObjects for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 OutsideObjectsMax;

	/**
	 * Triggers an event within the level based on the button that was pressed
	 *
	 * @param	CurrentButton		specifies the button that was pressed
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
	 * @param	CurrentSafe		specifies the safe that was clicked
	 */
	UFUNCTION()
	void SafeTriggered(ASafeActor* const CurrentSafe);

protected:
	/** Determines whether the initial narrative has been delivered or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bInitialNarrativeDelivered = false;

	/** Called to deliver the initial narrative */
	UFUNCTION()
	void DeliverInitialNarrative();

	/** Determines whether the barista narrative has been delivered or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bBaristaNarrativeDelivered = false;

	/** Called to deliver the barista narrative */
	UFUNCTION()
	void DeliverBaristaNarrative();

	/** Determines whether the objectives have been delivered or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bObjectivesDelivered = false;
	
	/** Called to deliver the objectives */
	UFUNCTION()
	void DeliverObjectives();

	/** Called to reveal the objectives */
	UFUNCTION()
	void RevealObjectives();

	/** Determines whether the cat has been checked on or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCheckedCat = false;

	/** Called to remove the narrative */
	UFUNCTION()
	void RemoveNarrative();

	/** Determines whether the house is clean or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCleanHouse = false;

	/** Determines whether the yard is clean or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCleanYard = false;

	/** Determines whether the memory game is unlocked or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMemoryGameUnlocked = false;
	
	/** Determines whether the scuba gear has been returned or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bScubaGearReturned = false;

	/** Stores the MemoryGameActor for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMemoryGameActor* MemoryGameActor;

	/** Stores the GravitySwitch for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AButtonActor* GravitySwitch;

	/**
	 * Finds all unique ButtonActors based on defined actor ID names
	 *
	 * @param	ButtonActors		specifies the array of ButtonActors to search
	 */
	UFUNCTION()
	void FindButtonActors(const TArray<AButtonActor*>& ButtonActors);
	
	/** Stores the Safe for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASafeActor* Safe;

	/**
	 * Finds all unique SafeActors based on defined actor ID names
	 *
	 * @param	SafeActors		specifies the array of SafeActors to search
	 */
	UFUNCTION()
	void FindSafeActors(const TArray<ASafeActor*>& SafeActors);
	
	/** Stores the InsideContainer for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* InsideContainer;

	/** Stores the InsideObjects for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASimulatedActor*> InsideObjects;

	/** Stores the OutsideContainer for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* OutsideContainer;

	/** Stores the OutsideObjects for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASimulatedActor*> OutsideObjects;

	/** Stores the ScubaGear for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* ScubaGear;

	/** Stores the LeineckersYard for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* LeineckersYard;

	/**
	 * Finds all unique SimulatedActors based on defined actor ID names
	 *
	 * @param	SimulatedActors		specifies the array of SimulatedActors to search
	 */
	UFUNCTION()
	void FindSimulatedActors(const TArray<ASimulatedActor*>& SimulatedActors);

	/**
	 * Called to check whether all simulated actors are in their correct containers
	 *
	 * @param	Objects			specifies the array of objects
	 * @param	Container		specifies the respective container
	 */
	UFUNCTION()
	void CheckObjectsInContainers(TArray<ASimulatedActor*>& Objects, ASimulatedActor* const Container);

	/** Called to attempt to unlock the memory game */
	UFUNCTION()
	void UnlockMemoryGame();
};
