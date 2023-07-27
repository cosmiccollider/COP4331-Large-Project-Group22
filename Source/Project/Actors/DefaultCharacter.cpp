// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCharacter.h"
#include "Actors/ButtonActor.h"
#include "Actors/DefaultActor.h"
#include "Actors/SimulatedActor.h"
#include "Actors/TransitionActor.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetSwitcher.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Levels/TestMapLevelScriptActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "UI/OverlayUserWidget.h"
#include "UI/PauseMenuUserWidget.h"
#include "UObject/ConstructorHelpers.h"

#define CREDITS_MAP "CreditsMap"
#define HOLD_DISTANCE 200
#define MAIN_MENU_MAP "MainMenuMap"
#define ROTATION_SENSITIVITY 0.5f
#define TRACE_DISTANCE 400

ADefaultCharacter::ADefaultCharacter()
{
 	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent and position it
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	Camera->bUsePawnControlRotation = true;

	// Create a PhysicsConstraint and PhysicsHandle
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComponent"));
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComponent"));

	// Set DefaultMappingContext to IMC_Default data asset
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultMappingContextFinder(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (DefaultMappingContextFinder.Succeeded())
	{
		DefaultMappingContext = DefaultMappingContextFinder.Object;
	}

	// Set CrouchAction to IA_Crouch data asset
	static ConstructorHelpers::FObjectFinder<UInputAction> CrouchActionFinder(TEXT("/Game/Input/Actions/IA_Crouch.IA_Crouch"));
	if (CrouchActionFinder.Succeeded())
	{
		CrouchAction = CrouchActionFinder.Object;
	}

	// Set JumpAction to IA_Jump data asset
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionFinder.Succeeded())
	{
		JumpAction = JumpActionFinder.Object;
	}

	// Set LookAction to IA_Look data asset
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder(TEXT("/Game/Input/Actions/IA_Look.IA_Look"));
	if (LookActionFinder.Succeeded())
	{
		LookAction = LookActionFinder.Object;
	}

	// Set MoveAction to IA_Move data asset
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("/Game/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionFinder.Succeeded())
	{
		MoveAction = MoveActionFinder.Object;
	}

	// Set PauseAction to IA_Pause data asset
	static ConstructorHelpers::FObjectFinder<UInputAction> PauseActionFinder(TEXT("/Game/Input/Actions/IA_Pause.IA_Pause"));
	if (PauseActionFinder.Succeeded())
	{
		PauseAction = PauseActionFinder.Object;
	}

	// Set PrimaryAction to IA_Primary data asset
	static ConstructorHelpers::FObjectFinder<UInputAction> PrimaryActionFinder(TEXT("/Game/Input/Actions/IA_Primary.IA_Primary"));
	if (PrimaryActionFinder.Succeeded())
	{
		PrimaryAction = PrimaryActionFinder.Object;
	}

	// Set SecondaryAction to IA_Secondary data asset
	static ConstructorHelpers::FObjectFinder<UInputAction> SecondaryActionFinder(TEXT("/Game/Input/Actions/IA_Secondary.IA_Secondary"));
	if (SecondaryActionFinder.Succeeded())
	{
		SecondaryAction = SecondaryActionFinder.Object;
	}

	// Set OverlayClass to OverlayWidgetBlueprint class
	static ConstructorHelpers::FClassFinder<UOverlayUserWidget> OverlayFinder(TEXT("/Game/UI/WBP_Overlay.WBP_Overlay_C"));
	if (OverlayFinder.Succeeded())
	{
		OverlayClass = OverlayFinder.Class;
	}

	// Set PauseMenuClass to PauseMenuWidgetBlueprint class
	static ConstructorHelpers::FClassFinder<UPauseMenuUserWidget> PauseMenuFinder(TEXT("/Game/UI/WBP_PauseMenu.WBP_PauseMenu_C"));
	if (PauseMenuFinder.Succeeded())
	{
		PauseMenuClass = PauseMenuFinder.Class;
	}
}

void ADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check that the overlay and reticle exist
	if (Overlay && Overlay->GetReticle())
	{
		// Check that the player is looking at a DefaultActor (or any of its subclasses) and change the color of the reticle to reflect that
		FHitResult OutHit;
		if (CanLineTrace(OutHit) && UKismetMathLibrary::ClassIsChildOf(OutHit.GetActor()->GetClass(), ADefaultActor::StaticClass()))
		{
			Overlay->SetReticleColor(FColor::Green);
		}
		else
		{
			Overlay->SetReticleColor(FColor::White);
		}
	}

	// Check that the player is grabbing or rotating an object to update its location or rotation
	if (bIsGrabbing)
	{
		MoveObject();

		if (bIsRotating)
		{
			RotateObject();
		}
	}
}

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

void ADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Add Overlay
		if (OverlayClass)
		{
			Overlay = CreateWidget<UOverlayUserWidget>(PC, OverlayClass);
			Overlay->AddToViewport();

			// Only add the reticle if the character is not in the main menu or credits
			FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
			if (CurrentLevel != MAIN_MENU_MAP && CurrentLevel != CREDITS_MAP)
			{
				Overlay->ShowReticle(true);
			}
		}
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
		// Center mouse so the player doesn't have to spend time finding the mouse when it becomes visible
		SetMouseCenter();

		// Create Pause Menu and add it to the screen
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
		if (PauseMenu->WidgetSwitcher->GetActiveWidgetIndex() == 0)
		{
			PauseMenu->Resume();
		}
		else if (PauseMenu->WidgetSwitcher->GetActiveWidgetIndex() == 1)
		{
			PauseMenu->Back();
		}
	}
}

void ADefaultCharacter::StartPrimary()
{
	FHitResult OutHit;

	EndLevel(OutHit);
	PressButton(OutHit);
	StartGrab(OutHit);
}

void ADefaultCharacter::StopPrimary()
{
	if (bIsGrabbing)
	{
		StopGrab();
	}
}

void ADefaultCharacter::StartSecondary()
{
	if (bIsGrabbing)
	{
		StartRotation();
	}
}

void ADefaultCharacter::StopSecondary()
{
	if (bIsRotating)
	{
		StopRotation();
	}
}

void ADefaultCharacter::EndLevel(FHitResult& OutHit)
{
	// Check that the actor in front of the player is a TransitionActor
	if (CanLineTrace(OutHit) && OutHit.GetActor()->GetClass() == ATransitionActor::StaticClass())
	{
		ATransitionActor* Actor = Cast<ATransitionActor>(OutHit.GetActor());

		// Call the function in DefaultLevelScriptActor that handles ending the level
		if (ADefaultLevelScriptActor* DefaultLevelScript = Cast<ADefaultLevelScriptActor>(GetWorld()->GetLevelScriptActor()))
		{
			DefaultLevelScript->EndLevel(Actor);
		}
	}
}

void ADefaultCharacter::PressButton(FHitResult& OutHit)
{
	// Check that the actor in front of the player is a ButtonActor
	if (CanLineTrace(OutHit) && OutHit.GetActor()->GetClass() == AButtonActor::StaticClass())
	{
		AButtonActor* Button = Cast<AButtonActor>(OutHit.GetActor());

		// Determine which level script is currently active and call the relative function for that level
		if (ATestMapLevelScriptActor* TestMapLevelScript = Cast<ATestMapLevelScriptActor>(GetWorld()->GetLevelScriptActor()))
		{
			TestMapLevelScript->ButtonPressed(Button);
		}
	}
}

void ADefaultCharacter::SetGravity(const bool bEnabled)
{
	if (bEnabled)
	{
		GetCharacterMovement()->GravityScale = 1;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 0;
	}
}

void ADefaultCharacter::StartGrab(FHitResult& OutHit)
{
	// Check that the actor in front of the player is a SimulatedActor
	if (CanLineTrace(OutHit) && OutHit.GetActor()->GetClass() == ASimulatedActor::StaticClass())
	{
		// Initialize a zeroed FRotator and grab the hit result with that rotation
		// A zeroed FRotator is necessary for the rotation function because the default FRotator() constructor does not initialize a value of 0
		// If it isn't initilized at 0, the rotation will be offset when starting to rotating, since rotating operates on a delta with the center of the screen
		FRotator Rotation = FRotator(0);
		PhysicsHandle->GrabComponentAtLocationWithRotation(OutHit.GetComponent(), FName(), OutHit.GetComponent()->GetComponentLocation(), Rotation);
		bIsGrabbing = true;
	}
}

void ADefaultCharacter::StopGrab()
{
	PhysicsHandle->ReleaseComponent();
	bIsGrabbing = false;
}

void ADefaultCharacter::MoveObject()
{
	// Get location as a distance from the camera based on current rotation
	FVector HoldDistance = Camera->GetComponentRotation().Vector() * HOLD_DISTANCE;
	FVector Location = Camera->GetComponentLocation() + HoldDistance;

	PhysicsHandle->SetTargetLocation(Location);
}

void ADefaultCharacter::StartRotation()
{
	// Center mouse to allow for the most possible rotation in any direction, since the mouse stops at the edge of the screen
	SetMouseCenter();

	bCanLook = false;
	bIsRotating = true;
}

void ADefaultCharacter::StopRotation()
{
	bIsRotating = false;
	bCanLook = true;
}

void ADefaultCharacter::RotateObject()
{
	// Get the current mouse position
	float MouseX;
	float MouseY;
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->GetMousePosition(MouseX, MouseY);

	// Set pitch and yaw according to the difference between current mouse position and viewport center
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	float Pitch = (MouseY - (ViewportSize.Y / 2.0f)) * ROTATION_SENSITIVITY;
	float Yaw = (MouseX - (ViewportSize.X / 2.0f)) * ROTATION_SENSITIVITY;

	// Set new rotation of object based on pitch and yaw
	PhysicsHandle->SetTargetRotation(UKismetMathLibrary::MakeRotator(0.0f, Pitch, Yaw));
}

void ADefaultCharacter::StartOverlayTransition()
{
	if (Overlay)
	{
		Overlay->PlayTransitionAnimation(EUMGSequencePlayMode::Forward);
	}
}

void ADefaultCharacter::SetMouseCenter()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

	PC->SetMouseLocation(UKismetMathLibrary::FTrunc(ViewportSize.X / 2.0f), UKismetMathLibrary::FTrunc(ViewportSize.Y / 2.0f));
}

bool ADefaultCharacter::CanLineTrace(FHitResult& OutHit)
{
	if (Camera)
	{
		// Set the distance to trace, which defines the delta from the start point (current location of the camera) to the end point
		FVector TraceDistance = Camera->GetComponentRotation().Vector() * TRACE_DISTANCE;
		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + TraceDistance;

		// Create a line trace and check that it finds a hit result
		bool LineTrace = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery(), false, TArray<AActor*>(), EDrawDebugTrace::None, OutHit, true);
		if (LineTrace && PhysicsHandle)
		{
			return true;
		}
	}

	return false;
}
