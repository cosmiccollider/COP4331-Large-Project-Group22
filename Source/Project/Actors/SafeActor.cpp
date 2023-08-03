// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SafeActor.h"
#include "Actors/DefaultCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "UI/SafeUserWidget.h"

ASafeActor::ASafeActor()
{
	// Set the default static mesh to the safe base
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SafeBaseFinder(TEXT("/Game/Props/SM_SafeBase.SM_SafeBase"));
	if (SafeBaseFinder.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(SafeBaseFinder.Object);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset not found: '/Game/Props/SM_SafeBase.SM_SafeBase'"));
	}

	// Create DoorStaticMeshComponent and attach the StaticMeshComponent to it
	DoorStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorStaticMeshComponent"));
	DoorStaticMeshComponent->SetupAttachment(StaticMeshComponent);

	// Set the door static mesh to the safe door
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SafeDoorFinder(TEXT("/Game/Props/SM_SafeDoor.SM_SafeDoor"));
	if (SafeDoorFinder.Succeeded())
	{
		DoorStaticMeshComponent->SetStaticMesh(SafeDoorFinder.Object);
		DoorStaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		DoorStaticMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset not found: '/Game/Props/SM_SafeDoor.SM_SafeDoor'"));
	}

	// Set the safe level sequence
	static ConstructorHelpers::FObjectFinder<ULevelSequence> SafeFinder(TEXT("/Game/Animations/LS_Safe.LS_Safe"));
	if (SafeFinder.Succeeded())
	{
		SafeLevelSequence = SafeFinder.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset not found: '/Game/Animations/LS_Safe.LS_Safe'"));
	}
}

void ASafeActor::BeginPlay()
{
	// Create a level sequence player to play the safe level sequence
	if (SafeLevelSequence)
	{
		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), SafeLevelSequence, FMovieSceneSequencePlaybackSettings(), LevelSequenceActor);
	}
}

void ASafeActor::SetCorrectCombination(const TArray<uint8>& Combination)
{
	CorrectCombination.Empty();

	for (int i = 0; i < Combination.Num(); i++)
	{
		CorrectCombination.Append(FString::FromInt(Combination[i]));
	}
}

void ASafeActor::AttemptSafe()
{
	if (!bSafeOpen)
	{
		ADefaultCharacter* PCharacter = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PController && PCharacter->SafeUIClass)
		{
			// Center mouse so the player doesn't have to spend time finding the mouse when it becomes visible
			PCharacter->SetMouseCenter();

			// Create Safe UI, add it to the screen, and assign the OwningSafe for this UI
			PCharacter->SafeUI = CreateWidget<USafeUserWidget>(PController, PCharacter->SafeUIClass);
			PCharacter->SafeUI->AddToViewport();
			PCharacter->SafeUI->OwningSafe = this;

			// Set input mode to UI so the player can only use the mouse, then show the mouse cursor
			FInputModeUIOnly InputMode;
			PController->SetInputMode(InputMode);
			PController->SetShowMouseCursor(true);

			// Set game to paused
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void ASafeActor::OpenSafe()
{
	if (LevelSequencePlayer)
	{
		LevelSequencePlayer->Play();
		bSafeOpen = true;
	}
}
