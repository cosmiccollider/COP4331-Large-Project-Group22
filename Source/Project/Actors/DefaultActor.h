// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultActor.generated.h"

class UMaterialInstanceDynamic;
class USoundBase;
class UStaticMeshComponent;

/**
 * DefaultActor provides a base class derived from Actor which we can make more specific actors from.
 */
UCLASS()
class PROJECT_API ADefaultActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADefaultActor();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Stores the DynamicMaterial for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* DynamicMaterial;

	/**
	 * Called to set a new base color for this actor using DynamicMaterial.
	 * Only works if the material on the actor has a "Base Color" parameter.
	 *
	 * @param	Color		specifies the FLinearColor to change the actor to
	 */
	UFUNCTION()
	void SetBaseColor(const FLinearColor Color);

	/**
	 * Called to set the glow effect for this actor using DynamicMaterial
	 *
	 * @param	Value		specifies the multiplier on the glow effect
	 */
	UFUNCTION()
	void SetGlow(const float Value);

	/** Stores the BlinkTimer handle for this actor */
	FTimerHandle BlinkTimer;

	/**
	 * Called to start a blinking glow effect for this actor
	 *
	 * @param	InRate		specifies the rate at which the actor will blink
	 * @param	Value		specifies the multiplier on the glow effect
	 */
	UFUNCTION()
	void StartBlinking(const float InRate, const float Value);

	/** Called to trigger a single blink event for this actor
	 *
	 * @param	Value		specifies the multiplier on the glow effect
	 */
	UFUNCTION()
	void Blink(const float Value);

	/** Called to stop a blinking glow effect for this actor */
	UFUNCTION()
	void StopBlinking();

	/** Stores the SoundBase for this actor */
	USoundBase* SoundBase;

	/** Stores the StaticMeshComponent for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	/** Returns the StaticMeshComponent of this actor */
	UFUNCTION()
	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }
};
