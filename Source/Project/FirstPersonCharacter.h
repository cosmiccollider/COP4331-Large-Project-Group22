// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "FirstPersonCharacter.generated.h"

class UInputComponent;
class UCameraComponent;

UCLASS()
class PROJECT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

	// First Person Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	// Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* JumpAction;

	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

protected:
	// Called for movement input
	void Move(const FInputActionValue& Value);

	// Called for looking input
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	// Returns FirstPersonCameraComponent subobject
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
