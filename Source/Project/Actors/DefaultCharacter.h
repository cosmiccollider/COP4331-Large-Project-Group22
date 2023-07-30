// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DefaultCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class UOverlayUserWidget;
class UPauseMenuUserWidget;
class UPhysicsConstraintComponent;
class UPhysicsHandleComponent;
class USafeUserWidget;

/**
 * DefaultCharacter represents the player controlled character.
 */
UCLASS()
class PROJECT_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADefaultCharacter();

//~==================================================
// Overrides

public:
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

//~==================================================
// Components

public:
	/** Stores the CameraComponent for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;

	/** Stores the PhysicsConstraintComponent for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint;

	/** Stores the PhysicsHandleComponent for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsHandleComponent* PhysicsHandle;

//~==================================================
// Input Actions

public:
	/** Stores the DefaultMappingContext for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	/** Stores the CrouchAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CrouchAction;

	/** Stores the JumpAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	/** Stores the LookAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	/** Stores the MoveAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	/** Stores the PauseAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseAction;

	/** Stores the PrimaryAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PrimaryAction;

	/** Stores the SecondaryAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SecondaryAction;

	/** Stores the ScrollAction for this actor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ScrollAction;

protected:
	/** Determines whether the player is currently able to look */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanLook = true;

	/** Called to toggle whether this character is crouching */
	UFUNCTION()
	void ToggleCrouch();

	/** Called to trigger the looking action */
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	/** Called to trigger the moving action */
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/** Called to open/close the pause menu */
	UFUNCTION()
	void Pause();

	/** Called to start a primary action */
	UFUNCTION()
	void StartPrimary();

	/** Called to stop a primary action */
	UFUNCTION()
	void StopPrimary();

	/** Called to start a secondary action */
	UFUNCTION()
	void StartSecondary();

	/** Called to stop a secondary action */
	UFUNCTION()
	void StopSecondary();

	/** Called to trigger the scroll action */
	UFUNCTION()
	void Scroll(const FInputActionValue& Value);

//~==================================================
// User Widgets

public:
	/** Stores the class of OverlayUserWidget */
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UOverlayUserWidget> OverlayClass;

	/** Stores the OverlayUserWidget for this actor */
	UPROPERTY()
	UOverlayUserWidget* Overlay;

	/** Returns the OverlayUserWidget for this actor */
	UFUNCTION()
	UOverlayUserWidget* GetOverlay() const { return Overlay; };
	
	/** Determines whether the player is currently in the pause menu */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInPauseMenu = false;

	/** Stores the class of PauseMenu */
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UPauseMenuUserWidget> PauseMenuClass;

	/** Stores the PauseMenu for this actor */
	UPROPERTY()
	UPauseMenuUserWidget* PauseMenu;

	/** Stores the class of SafeUI */
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<USafeUserWidget> SafeUIClass;

	/** Stores the Safe UI for this actor */
	UPROPERTY()
	USafeUserWidget* SafeUI;

//~==================================================
// Abilities

public:
	/**
	 * Sets whether or not this character is affected by gravity
	 *
	 * @param	bEnabled	specify true to enable gravity
	 */
	UFUNCTION()
	void SetGravity(const bool bEnabled);

protected:
	/**
	 * Allows this character to trigger ButtonActor events within a level
	 *
	 * @param	OutHit		specifies a hit result from a line trace
	 */
	UFUNCTION()
	void EndLevel(FHitResult& OutHit);

	/**
	 * Allows this character to trigger ButtonActor events within a level
	 *
	 * @param	OutHit		specifies a hit result from a line trace
	 */
	UFUNCTION()
	void TriggerButton(FHitResult& OutHit);

	/**
	 * Allows this character to trigger MemoryGameActor events within a level
	 *
	 * @param	OutHit		specifies a hit result from a line trace
	 */
	UFUNCTION()
	void TriggerMemoryGame(FHitResult& OutHit);

	/**
	 * Allows this character to trigger SafeActor events within a level
	 *
	 * @param	OutHit		specifies a hit result from a line trace
	 */
	UFUNCTION()
	void TriggerSafe(FHitResult& OutHit);

	/** Determines whether the player is currently grabbing an actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGrabbing = false;

	/**
	 * Allows this character to start grabbing an actor within the level
	 *
	 * @param	OutHit		specifies a hit result from a line trace
	 */
	UFUNCTION()
	void StartGrab(FHitResult& OutHit);

	/** Triggered to stop grabbing an actor that's currently being grabbed */
	UFUNCTION()
	void StopGrab();

	/** Updates the location of an actor that's currently being grabbed */
	UFUNCTION()
	void MoveObject();

	/** Determines whether the player is currently rotating an actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRotating = false;

	/** Allows this character to start rotating an actor that's currently being grabbed */
	UFUNCTION()
	void StartRotation();

	/** Triggered to stop rotating an actor that's currently being rotated */
	UFUNCTION()
	void StopRotation();

	/** Updates the rotation of an actor that's currently being rotated */
	UFUNCTION()
	void RotateObject();

	/** Determines how far away the player holds an object that's currently being grabbed (in centimeters) */
	UPROPERTY()
	int32 HoldDistance = 200;

	/**
	 * Set the HoldDistance, which determines how far away the player holds an object
	 *
	 * @param	Direction		specifies whether to bring the object closer or push it further away (input should be -1 or 1)
	 */
	UFUNCTION()
	void SetHoldDistance(const int8 Direction);

//~==================================================
// Helpers

public:
	/** Starts the transition animation for this character's overlay */
	void StartOverlayTransition();
	
	/** Updates the location of the mouse to be the center of the screen */
	UFUNCTION()
	void SetMouseCenter();

protected:
	/**
	 * Attempts to draw a line trace from the camera to a set distance in front of the character
	 *
	 * @param	OutHit		specifies a hit result from the line trace
	 */
	UFUNCTION()
	bool CanLineTrace(FHitResult& OutHit);
};
