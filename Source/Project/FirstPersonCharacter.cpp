// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "PauseMenuUserWidget.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Set default input mapping and actions
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextFinder(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingContextFinder.Succeeded()) DefaultMappingContext = DefaultMappingContextFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> CrouchActionFinder(TEXT("/Game/Input/Actions/IA_Crouch.IA_Crouch"));
	if (CrouchActionFinder.Succeeded()) CrouchAction = CrouchActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionFinder.Succeeded()) JumpAction = JumpActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder(TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
	if (LookActionFinder.Succeeded()) LookAction = LookActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionFinder.Succeeded()) MoveAction = MoveActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> PauseActionFinder(TEXT("/Game/Input/Actions/IA_Pause.IA_Pause"));
	if (PauseActionFinder.Succeeded()) PauseAction = PauseActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> PrimaryActionFinder(TEXT("/Game/Input/Actions/IA_Primary.IA_Primary"));
	if (PrimaryActionFinder.Succeeded()) PrimaryAction = PrimaryActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> SecondaryActionFinder(TEXT("/Game/Input/Actions/IA_Secondary.IA_Secondary"));
	if (SecondaryActionFinder.Succeeded()) SecondaryAction = SecondaryActionFinder.Object;

	// Set Class Variables
	inPauseMenu = false;
	//isGrabbing = false;
	//isRotating = false;

	// Set Pause Menu Widget Class
	static ConstructorHelpers::FClassFinder<UPauseMenuUserWidget> PauseMenuFinder(TEXT("/Game/UI/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (PauseMenuFinder.Succeeded()) PauseMenuClass = PauseMenuFinder.Class;
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Crouch
		Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &AFirstPersonCharacter::ToggleCrouch);

		// Jump
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Look
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::Look);

		// Move
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonCharacter::Move);

		// Pause
		Input->BindAction(PauseAction, ETriggerEvent::Started, this, &AFirstPersonCharacter::Pause);

		// Primary
		Input->BindAction(PrimaryAction, ETriggerEvent::Started, this, &AFirstPersonCharacter::StartPrimary);
		Input->BindAction(PrimaryAction, ETriggerEvent::Completed, this, &AFirstPersonCharacter::StopPrimary);

		// Secondary
		Input->BindAction(SecondaryAction, ETriggerEvent::Started, this, &AFirstPersonCharacter::StartSecondary);
		Input->BindAction(SecondaryAction, ETriggerEvent::Completed, this, &AFirstPersonCharacter::StopSecondary);
	}
}

void AFirstPersonCharacter::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
		Crouch();
	}
}

void AFirstPersonCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFirstPersonCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFirstPersonCharacter::Pause()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PauseMenuClass && !inPauseMenu)
	{
		PauseMenu = CreateWidget<UPauseMenuUserWidget>(PC, PauseMenuClass);
		PauseMenu->AddToViewport();
		FInputModeGameAndUI InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		inPauseMenu = true;
	}
	else if (PauseMenu)
	{
		PauseMenu->RemoveFromParent();
		PauseMenu = nullptr;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		inPauseMenu = false;
	}
}

void AFirstPersonCharacter::StartPrimary()
{
}

void AFirstPersonCharacter::StopPrimary()
{
}

void AFirstPersonCharacter::StartSecondary()
{
}

void AFirstPersonCharacter::StopSecondary()
{
}
