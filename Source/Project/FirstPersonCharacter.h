// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FirstPersonCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class UPauseMenuUserWidget;

UCLASS()
class PROJECT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();

	// Class Variables
	bool inPauseMenu;
	//bool isGrabbing;
	//bool isRotating;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseMenuUserWidget> PauseMenuClass;

	UPROPERTY()
		UPauseMenuUserWidget* PauseMenu;

	// First Person Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FirstPersonCameraComponent;

	// Default Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* DefaultMappingContext;

	// Crouch Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* CrouchAction;

	// Jump Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;

	// Look Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* LookAction;

	// Move Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* MoveAction;

	// Pause Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* PauseAction;

	// Primary Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* PrimaryAction;

	// Secondary Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* SecondaryAction;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns FirstPersonCameraComponent subobject
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	// Called for crouch input
	void ToggleCrouch();

	// Called for look input
	void Look(const FInputActionValue& Value);

	// Called for move input
	void Move(const FInputActionValue& Value);

	// Called for pause input
	void Pause();

	// Called for primary input
	void StartPrimary();
	void StopPrimary();

	// Called for secondary input
	void StartSecondary();
	void StopSecondary();
};
