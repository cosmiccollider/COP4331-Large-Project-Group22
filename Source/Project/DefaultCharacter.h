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
class UPauseMenuUserWidget;
class UPhysicsConstraintComponent;
class UPhysicsHandleComponent;

UCLASS()
class PROJECT_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

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

	// Physics Constraint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPhysicsConstraintComponent* PhysicsConstraint;

	// Physics Handle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPhysicsHandleComponent* PhysicsHandle;

	// Pause Menu
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UPauseMenuUserWidget> PauseMenuClass;

	UPROPERTY()
		UPauseMenuUserWidget* PauseMenu;

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

	// Class Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanLook;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bInPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsGrabbing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsRotating;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input Action functions
	void ToggleCrouch();
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Pause();
	void StartPrimary();
	void StopPrimary();
	void StartSecondary();
	void StopSecondary();
};
