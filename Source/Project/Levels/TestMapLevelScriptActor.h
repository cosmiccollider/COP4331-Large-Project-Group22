// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/DefaultLevelScriptActor.h"
#include "TestMapLevelScriptActor.generated.h"

class AButtonActor;
class ASimulatedActor;
class ATransitionActor;

/**
 * TestMapLevelScriptActor provides the level specific capabilities of the TestMap.
 */
UCLASS()
class PROJECT_API ATestMapLevelScriptActor : public ADefaultLevelScriptActor
{
	GENERATED_BODY()
	
public:
	ATestMapLevelScriptActor();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Stores the GravitySwitch for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AButtonActor* GravitySwitch;

	/** Stores the TestCube for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* TestCube;

	/** Stores the TestBox for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* TestBox;

	/** Stores the GravitySwitch for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATransitionActor* Star;

	/**
	 * Triggers an event within the level based on the button that was pressed
	 *
	 * @param	Button		specifies the specific button that was pressed
	 */
	UFUNCTION()
	void ButtonPressed(AButtonActor* const Button);

	/**
	 * Called to start the ending process for the current level upon clicking an actor
	 *
	 * @param	Actor		specifies the TransitionActor that ends the level
	 */
	UFUNCTION()
	void EndLevel(ATransitionActor* const Actor);

	/**
	 * Finds all unique ButtonActors based on defined actor ID names
	 *
	 * @param	ButtonActors		specifies the array of ButtonActors to search
	 */
	UFUNCTION()
	void FindButtonActors(const TArray<AButtonActor*>& ButtonActors);

	/**
	 * Finds all unique SimulatedActors based on defined actor ID names
	 *
	 * @param	SimulatedActors		specifies the array of SimulatedActors to search
	 */
	UFUNCTION()
	void FindSimulatedActors(const TArray<ASimulatedActor*>& SimulatedActors);

	/** Called to open the next level */
	void NextLevel();
};
