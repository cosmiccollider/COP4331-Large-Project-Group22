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
	/** Stores the GravitySwitch for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AButtonActor* GravitySwitch;

	/** Stores the MemoryGameActor for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AMemoryGameActor* MemoryGameActor;

	/** Stores the Safe for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASafeActor* Safe;

	/** Stores the PlaceholderObject for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* PlaceholderObject;

	/** Stores the PlaceholderContainer for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* PlaceholderContainer;

	/**
	 * Triggers an event within the level based on the button that was pressed
	 *
	 * @param	Button		specifies the specific button that was pressed
	 */
	UFUNCTION()
	void ButtonTriggered(AButtonActor* const CurrentButton);

	/**
	 * Triggers an event within the level based on the safe that was pressed
	 *
	 * @param	Safe		specifies the specific safe that was pressed
	 */
	UFUNCTION()
	void SafeTriggered(ASafeActor* const CurrentSafe);

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
};
