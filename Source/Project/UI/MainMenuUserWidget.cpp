// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"
#include "Actors/DefaultCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/OverlayUserWidget.h"

#define TEST_MAP "TestMap"

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind PlayButton to Play() function
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Play);
	}

	// Bind QuitButton to Quit() function
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Quit);
	}
}

void UMainMenuUserWidget::Play()
{
	StartTransition(EMainMenuButton::Play);
}

void UMainMenuUserWidget::Quit()
{
	StartTransition(EMainMenuButton::Quit);
}

void UMainMenuUserWidget::StartTransition(const EMainMenuButton Button)
{
	// Play the overlay transition animation
	if (ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		PC->StartOverlayTransition();
	}

	// Set a timer to change the map after the transition animation completes
	FTimerHandle TransitionTimer;
	FTimerDelegate MapTransition = FTimerDelegate::CreateUFunction(this, FName("ChangeLevel"), Button);
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, MapTransition, 1.0f, false);
}

void UMainMenuUserWidget::ChangeLevel(const EMainMenuButton Button)
{
	if (Button == EMainMenuButton::Play)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEST_MAP);
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	else if (Button == EMainMenuButton::Quit)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
	}
}
