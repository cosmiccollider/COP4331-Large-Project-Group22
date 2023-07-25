// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"
#include "Actors/DefaultCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/OverlayUserWidget.h"

#define MAIN_MENU_MAP "MainMenuMap"

void UPauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind ResumeButton to Resume() function
	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Resume);
	}

	// Bind RestartButton to Restart() function
	if (RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Restart);
	}

	// Bind MainMenuButton to MainMenu() function
	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::MainMenu);
	}

	// Bind DesktopButton to Desktop() function
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
	StartTransition(EPauseMenuButton::Restart);
}

void UPauseMenuUserWidget::MainMenu()
{
	StartTransition(EPauseMenuButton::MainMenu);
}

void UPauseMenuUserWidget::Desktop()
{
	StartTransition(EPauseMenuButton::Desktop);
}

void UPauseMenuUserWidget::StartTransition(const EPauseMenuButton Button)
{
	// Play the overlay transition animation
	if (ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		PC->StartOverlayTransition();
	}

	// Unpause the game to allow a timer to start counting
	Resume();
	
	// Set a timer to change the map after the transition animation completes
	FTimerHandle TransitionTimer;
	FTimerDelegate MapTransition = FTimerDelegate::CreateUFunction(this, FName("ChangeLevel"), Button);
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, MapTransition, 1.0f, false);
}

void UPauseMenuUserWidget::ChangeLevel(const EPauseMenuButton Button)
{
	if (Button == EPauseMenuButton::Restart)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(GetWorld())));
	}
	else if (Button == EPauseMenuButton::MainMenu)
	{
		UGameplayStatics::OpenLevel(GetWorld(), MAIN_MENU_MAP);
	}
	else if (Button == EPauseMenuButton::Desktop)
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
	}
}
