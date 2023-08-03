// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"
#include "Actors/DefaultCharacter.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/OverlayUserWidget.h"

#define MAIN_MENU_MAP "MainMenuMap"

void UPauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Play the animation to open the pause menu
	UUserWidget::PlayAnimationForward(PausePanelAnimation);

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

	// Bind SettingsButton to Settings() function
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Settings);
	}
	
	// Bind BackButton to Back() function
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Back);
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
	StartPanelTransition(EPauseMenuButton::Resume);
}

void UPauseMenuUserWidget::Restart()
{
	StartLevelTransition(EPauseMenuButton::Restart);
}

void UPauseMenuUserWidget::Settings()
{
	StartPanelTransition(EPauseMenuButton::Settings);
}

void UPauseMenuUserWidget::Back()
{
	StartPanelTransition(EPauseMenuButton::Back);
}

void UPauseMenuUserWidget::MainMenu()
{
	StartLevelTransition(EPauseMenuButton::MainMenu);
}

void UPauseMenuUserWidget::Desktop()
{
	StartLevelTransition(EPauseMenuButton::Desktop);
}

void UPauseMenuUserWidget::StartLevelTransition(const EPauseMenuButton Button)
{
	if (ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		// Start the overlay transition for the player character and get the duration of that animation
		PC->StartOverlayTransition();
		float LevelTransitionDuration = PC->Overlay->TransitionAnimation->GetEndTime();
		
		// Unpause the game to allow a timer to start counting
		Resume();
	
		// Set a timer to change the level after the transition animation completes
		FTimerHandle TransitionTimer;
		FTimerDelegate ChangeLevel = FTimerDelegate::CreateUFunction(this, FName("ChangeLevel"), Button);
		GetWorld()->GetTimerManager().SetTimer(TransitionTimer, ChangeLevel, LevelTransitionDuration, false);
	}
}

void UPauseMenuUserWidget::StartPanelTransition(const EPauseMenuButton Button)
{
	float PanelTransitionDuration = 0.0f;

	// Play the panel animation based on the button that was pressed
	if (Button == EPauseMenuButton::Resume || Button == EPauseMenuButton::Settings)
	{
		UUserWidget::PlayAnimationReverse(PausePanelAnimation);
		PanelTransitionDuration = PausePanelAnimation->GetEndTime();
	}
	else if (Button == EPauseMenuButton::Back)
	{
		UUserWidget::PlayAnimationReverse(SettingsPanelAnimation);
		PanelTransitionDuration = SettingsPanelAnimation->GetEndTime();
	}

	// Unpause the game to allow a timer to start counting
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// Set a timer to change the panel after the animation completes
	FTimerHandle PanelTimer;
	FTimerDelegate ChangePanel = FTimerDelegate::CreateUFunction(this, FName("ChangePanel"), Button);
	GetWorld()->GetTimerManager().SetTimer(PanelTimer, ChangePanel, PanelTransitionDuration, false);
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

void UPauseMenuUserWidget::ChangePanel(const EPauseMenuButton Button)
{
	// Pause the game since the timer is done
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// Resume game if resume button was pressed, otherwise change the panel
	if (Button == EPauseMenuButton::Resume)
	{
		ResumeGame();
	}
	else if (Button == EPauseMenuButton::Settings)
	{
		UMenuUserWidget::Settings();
		UUserWidget::PlayAnimationForward(SettingsPanelAnimation);
	}
	else if (Button == EPauseMenuButton::Back)
	{
		UMenuUserWidget::Back();
		UUserWidget::PlayAnimationForward(PausePanelAnimation);
	}
}

void UPauseMenuUserWidget::ResumeGame()
{
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ADefaultCharacter* PCharacter = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PCharacter->PauseMenu)
	{
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
}
