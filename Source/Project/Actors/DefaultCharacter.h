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

UCLASS()
class PROJECT_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

// ==================== Character ====================

public:
	// Sets default values for this character's properties
	ADefaultCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// First Person Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;

	// Overlay User Widget
	UFUNCTION()
	UOverlayUserWidget* GetOverlay();

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UOverlayUserWidget> OverlayClass;

	UPROPERTY()
	UOverlayUserWidget* Overlay;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ==================== Input Actions ====================

public:
	// Default Mapping Context and Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* PrimaryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SecondaryAction;

	// Pause Menu User Widget
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UPauseMenuUserWidget> PauseMenuClass;

	UPROPERTY()
	UPauseMenuUserWidget* PauseMenu;

	// Input Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInPauseMenu;

protected:
	// Input Action functions
	UFUNCTION()
	void ToggleCrouch();

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Pause();

	UFUNCTION()
	void StartPrimary();

	UFUNCTION()
	void StopPrimary();

	UFUNCTION()
	void StartSecondary();

	UFUNCTION()
	void StopSecondary();

	// Input variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanLook;

// ==================== Physics ====================

public:
	// Physics Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPhysicsHandleComponent* PhysicsHandle;

	// Gravity
	UFUNCTION()
	void SetGravity(bool bEnabled);

protected:
	// Grab Object
	UFUNCTION()
	void StartGrab();

	UFUNCTION()
	void StopGrab();

	UFUNCTION()
	void MoveObject();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGrabbing;

	// Rotate Object
	UFUNCTION()
	void StartRotation();

	UFUNCTION()
	void StopRotation();

	UFUNCTION()
	void RotateObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRotating;

// ==================== Helpers ====================

protected:
	// Set Mouse Center
	UFUNCTION()
	void SetMouseCenter();
};
