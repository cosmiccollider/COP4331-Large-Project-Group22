// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/DefaultLevelScriptActor.h"
#include "TestMapLevelScriptActor.generated.h"

class AButtonActor;
class ASimulatedActor;

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

	/** Stores the GravitySwitch for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AButtonActor* GravitySwitch;

	/** Stores the TestCube for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* TestCube;

	/** Stores the TestBox for this level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ASimulatedActor* TestBox;

	/**
	 * Triggers an event within the level based on the button that was pressed
	 *
	 * @param	Button		specifies the specific button that was pressed
	 */
	UFUNCTION()
	void ButtonPressed(AButtonActor* const Button);

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
};
