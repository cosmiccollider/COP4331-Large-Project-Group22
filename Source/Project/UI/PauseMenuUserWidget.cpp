// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"
#include "Components/Button.h"
#include "Actors/DefaultCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#define MAIN_MENU_MAP "MainMenuMap"

// Override Native Construct
void UPauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Resume);
	}

	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Restart);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::MainMenu);
	}

	if (DesktopButton)
	{
		DesktopButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Desktop);
	}
}

void UPauseMenuUserWidget::Resume()
{
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ADefaultCharacter* PCharacter = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Remove Pause Menu from the screen
	PCharacter->PauseMenu->RemoveFromParent();
	PCharacter->PauseMenu = nullptr;

	// Set input mode to Game and hide the mouse cursor
	FInputModeGameOnly InputMode;
	PController->SetInputMode(InputMode);
	PController->SetShowMouseCursor(false);

	// Set game to unpaused
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	PCharacter->bInPauseMenu = false;
}

void UPauseMenuUserWidget::Restart()
{
	Resume();
	UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(GetWorld())));
}

void UPauseMenuUserWidget::MainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MAIN_MENU_MAP);
}

void UPauseMenuUserWidget::Desktop()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
