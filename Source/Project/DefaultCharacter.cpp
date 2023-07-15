// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PauseMenuUserWidget.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "UObject/ConstructorHelpers.h"

#define HOLD_DISTANCE 200;
#define ROTATION_MODIFIER 2.0f;
#define TRACE_DISTANCE 400;

// Sets default values
ADefaultCharacter::ADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	Camera->bUsePawnControlRotation = true;

	// Create a physics constraint
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComponent"));

	// Create a physics handle
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));

	// Set Default Mapping Context and Input Action default assets
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
	bCanLook = true;
	bInPauseMenu = false;
	bIsGrabbing = false;
	bIsRotating = false;

	// Set Pause Menu Widget Class
	static ConstructorHelpers::FClassFinder<UPauseMenuUserWidget> PauseMenuFinder(TEXT("/Game/UI/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (PauseMenuFinder.Succeeded()) PauseMenuClass = PauseMenuFinder.Class;
}

// Called when the game starts or when spawned
void ADefaultCharacter::BeginPlay()
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

// Called every frame
void ADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsGrabbing)
	{
		FVector HoldDistance = Camera->GetComponentRotation().Vector() * HOLD_DISTANCE;
		FVector Location = Camera->GetComponentLocation() + HoldDistance;
		PhysicsHandle->SetTargetLocation(Location);

		if (bIsRotating)
		{
			float MouseX;
			float MouseY;
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			PC->GetMousePosition(MouseX, MouseY);
			FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
			float Pitch = (MouseY - (ViewportSize.Y / 2.0f)) / ROTATION_MODIFIER;
			float Yaw = (MouseX - (ViewportSize.X / 2.0f)) / ROTATION_MODIFIER;
			PhysicsHandle->SetTargetRotation(UKismetMathLibrary::MakeRotator(0.0f, Pitch, Yaw));
		}
	}
}

// Called to bind functionality to input
void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Crouch
		Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADefaultCharacter::ToggleCrouch);

		// Jump
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Look
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADefaultCharacter::Look);

		// Move
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultCharacter::Move);

		// Pause
		Input->BindAction(PauseAction, ETriggerEvent::Started, this, &ADefaultCharacter::Pause);

		// Primary
		Input->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ADefaultCharacter::StartPrimary);
		Input->BindAction(PrimaryAction, ETriggerEvent::Completed, this, &ADefaultCharacter::StopPrimary);

		// Secondary
		Input->BindAction(SecondaryAction, ETriggerEvent::Started, this, &ADefaultCharacter::StartSecondary);
		Input->BindAction(SecondaryAction, ETriggerEvent::Completed, this, &ADefaultCharacter::StopSecondary);
	}
}

void ADefaultCharacter::ToggleCrouch()
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

void ADefaultCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller && bCanLook)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADefaultCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// Add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADefaultCharacter::Pause()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PauseMenuClass && !bInPauseMenu)
	{
		// Set mouse location to the center of the screen
		// This is done to so the player doesn't have to spend time finding the mouse when it becomes visible
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
		PC->SetMouseLocation(UKismetMathLibrary::FTrunc(ViewportSize.X / 2.0f), UKismetMathLibrary::FTrunc(ViewportSize.Y / 2.0f));

		// Create Pause Menu widget and add it to the screen
		PauseMenu = CreateWidget<UPauseMenuUserWidget>(PC, PauseMenuClass);
		PauseMenu->AddToViewport();

		// Set input mode to Game and UI so the player can use mouse or keyboard inputs, then show the mouse cursor
		FInputModeGameAndUI InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(true);

		// Set game to paused
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bInPauseMenu = true;
	}
	else if (PauseMenu)
	{
		// Remove Pause Menu from the screen
		PauseMenu->RemoveFromParent();
		PauseMenu = nullptr;

		// Set input mode to Game and hide the mouse cursor
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetShowMouseCursor(false);

		// Set game to unpaused
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bInPauseMenu = false;
	}
}

void ADefaultCharacter::StartPrimary()
{
	if (Camera)
	{
		// Set the distance to trace, which defines the delta from the start point (current location of the camera) to the end point
		FVector TraceDistance = Camera->GetComponentRotation().Vector() * TRACE_DISTANCE;
		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + TraceDistance;

		// Check that the line trace finds a hit result
		FHitResult OutHit;
		bool LineTrace = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery(), false, TArray<AActor*>(), EDrawDebugTrace::ForDuration, OutHit, true);
		if (LineTrace && PhysicsHandle)
		{
			// Initialize a zeroed FRotator and grab the hit result with that rotation
			// A zeroed FRotator is necessary for the rotation function because the default FRotator() constructor does not initialize a value of 0
			// If it isn't initilized at 0, the rotation will be offset when starting to rotating, since rotating operates on a delta with the center of the screen
			FRotator Rotation = FRotator(0);
			PhysicsHandle->GrabComponentAtLocationWithRotation(OutHit.GetComponent(), FName(), OutHit.GetComponent()->GetComponentLocation(), Rotation);
			bIsGrabbing = true;
		}
	}
}

void ADefaultCharacter::StopPrimary()
{
	// If an object is currently grabbed, release it
	if (bIsGrabbing)
	{
		PhysicsHandle->ReleaseComponent();
		bIsGrabbing = false;
	}
}

void ADefaultCharacter::StartSecondary()
{
	// Set mouse location to the center of the screen
	// This is done to allow for the most possible rotation in any direction, since the mouse stops at the edge of the screen
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	PC->SetMouseLocation(UKismetMathLibrary::FTrunc(ViewportSize.X / 2.0f), UKismetMathLibrary::FTrunc(ViewportSize.Y / 2.0f));

	// Disable looking and enable rotating
	bCanLook = false;
	bIsRotating = true;
}

void ADefaultCharacter::StopSecondary()
{
	// Disable rotating and enable looking
	bIsRotating = false;
	bCanLook = true;
}
