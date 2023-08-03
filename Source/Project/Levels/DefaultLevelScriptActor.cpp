// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/DefaultLevelScriptActor.h"
#include "Actors/ButtonActor.h"
#include "Actors/DefaultActor.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/MemoryGameActor.h"
#include "Actors/SafeActor.h"
#include "Actors/SimulatedActor.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/OverlayUserWidget.h"

#define CREDITS_MAP "CreditsMap"
#define HOUSE_MAP "HouseMap"
#define MAIN_MENU_MAP "MainMenuMap"
#define TEST_MAP "TestMap"

ADefaultLevelScriptActor::ADefaultLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefaultLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// Get all DefaultActors in the level and sort them into subclass arrays
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADefaultActor::StaticClass(), ActorArray);
	SortActors(ActorArray);
}

void ADefaultLevelScriptActor::EndLevel(ATransitionActor* const Actor, const ENextLevel Level)
{
	// For normal levels, we ensure that the Actor being passed is actually the TransitionActor
	// For levels where a TransitionActor is not spawned, simply proceed
	if (Actor == TransitionActor
	|| UGameplayStatics::GetCurrentLevelName(GetWorld()) == CREDITS_MAP)
	{
		// Play the overlay transition animation
		if (ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			PC->StartOverlayTransition();
			float TransitionDuration = PC->GetOverlay()->TransitionAnimation->GetEndTime();

			// Set a timer to open the next level after the transition animation completes
			FTimerHandle TransitionTimer;
			FTimerDelegate NextLevel = FTimerDelegate::CreateUFunction(this, FName("NextLevel"), Level);
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, NextLevel, TransitionDuration, false);
		}
	}
}

bool ADefaultLevelScriptActor::IsObjectInContainer(ASimulatedActor* const Object, ASimulatedActor* const Container)
{
	if (Object && Container)
	{
		FVector ObjectOrigin = Object->GetActorLocation();

		FVector ContainerOrigin;
		FVector ContainerBoxExtent;
		Container->GetActorBounds(false, ContainerOrigin, ContainerBoxExtent);

		// Add a small amount to extent on the Z axis to allow for slight inconsistencies
		ContainerBoxExtent = FVector(ContainerBoxExtent.X, ContainerBoxExtent.Y, ContainerBoxExtent.Z * 1.2);

		return UKismetMathLibrary::IsPointInBox(ObjectOrigin, ContainerOrigin, ContainerBoxExtent);
	}

	return false;
}

void ADefaultLevelScriptActor::NextLevel(const ENextLevel Level)
{
	if (Level == ENextLevel::Secret)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEST_MAP);
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	else
	{
		FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
		
		if (CurrentLevel == HOUSE_MAP)
		{
			UGameplayStatics::OpenLevel(GetWorld(), CREDITS_MAP);
		}
		else if (CurrentLevel == TEST_MAP)
		{
			UGameplayStatics::OpenLevel(GetWorld(), CREDITS_MAP);
		}
		else if (CurrentLevel == CREDITS_MAP)
		{
			UGameplayStatics::OpenLevel(GetWorld(), MAIN_MENU_MAP);
		}
	}
}

void ADefaultLevelScriptActor::SetGravity(const bool bEnabled)
{
	// Set gravity for player
	ADefaultCharacter* Player = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetGravity(bEnabled);

	// Set gravity for objects
	for (ASimulatedActor* SimulatedActor : SimulatedActorArray) {
		SimulatedActor->SetGravity(bEnabled);
	}
}

void ADefaultLevelScriptActor::SortActors(const TArray<AActor*>& Actors)
{
	for (AActor* Actor : ActorArray)
	{
		// Cast each Actor to each subclass of DefaultActor
		// If the cast fails, the type doesn't match so it checks the next type
		// If the cast succeeds, we've found the correct type so we add it to the subclass array
		if (AButtonActor* ButtonActor = Cast<AButtonActor>(Actor))
		{
			ButtonActorArray.Add(ButtonActor);
		}
		else if (AMemoryGameActor* MemoryGameActor = Cast<AMemoryGameActor>(Actor))
		{
			MemoryGameActorArray.Add(MemoryGameActor);
		}
		else if (ASafeActor* SafeActor = Cast<ASafeActor>(Actor))
		{
			SafeActorArray.Add(SafeActor);
		}
		else if (ASimulatedActor* SimulatedActor = Cast<ASimulatedActor>(Actor))
		{
			SimulatedActorArray.Add(SimulatedActor);
		}
	}
}
