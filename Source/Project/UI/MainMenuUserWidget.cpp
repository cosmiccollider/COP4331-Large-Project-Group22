// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"
#include "Actors/DefaultCharacter.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/OverlayUserWidget.h"

#define TEST_MAP "TestMap"

void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Play the animation to open the main menu
	UUserWidget::PlayAnimationForward(MainMenuPanelAnimation);

	// Bind PlayButton to Play() function
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Play);
	}

	// Bind SettingsButton to Settings() function
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Settings);
	}

	// Bind BackButton to Back() function
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Back);
	}

	// Bind QuitButton to Quit() function
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Quit);
	}
}

void UMainMenuUserWidget::Play()
{
	StartLevelTransition(EMainMenuButton::Play);
}

void UMainMenuUserWidget::Settings()
{
	StartPanelTransition(EMainMenuButton::Settings);
}

void UMainMenuUserWidget::Back()
{
	StartPanelTransition(EMainMenuButton::Back);
}

void UMainMenuUserWidget::Quit()
{
	StartLevelTransition(EMainMenuButton::Quit);
}

void UMainMenuUserWidget::StartLevelTransition(const EMainMenuButton Button)
{
	if (ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		// Start the overlay transition for the player character and get the duration of that animation
		PC->StartOverlayTransition();
		float LevelTransitionDuration = PC->Overlay->TransitionAnimation->GetEndTime();
		
		// Set a timer to change the map after the transition animation completes
		FTimerHandle TransitionTimer;
		FTimerDelegate ChangeLevel = FTimerDelegate::CreateUFunction(this, FName("ChangeLevel"), Button);
		GetWorld()->GetTimerManager().SetTimer(TransitionTimer, ChangeLevel, LevelTransitionDuration, false);
	}
}

void UMainMenuUserWidget::StartPanelTransition(const EMainMenuButton Button)
{
	float PanelTransitionDuration = 0.0f;

	// Play the panel animation based on the button that was pressed
	if (Button == EMainMenuButton::Play || Button == EMainMenuButton::Settings)
	{
		UUserWidget::PlayAnimationReverse(MainMenuPanelAnimation);
		PanelTransitionDuration = MainMenuPanelAnimation->GetEndTime();
	}
	else if (Button == EMainMenuButton::Back)
	{
		UUserWidget::PlayAnimationReverse(SettingsPanelAnimation);
		PanelTransitionDuration = SettingsPanelAnimation->GetEndTime();
	}

	// Set a timer to change the panel after the animation completes
	FTimerHandle PanelTimer;
	FTimerDelegate ChangePanel = FTimerDelegate::CreateUFunction(this, FName("ChangePanel"), Button);
	GetWorld()->GetTimerManager().SetTimer(PanelTimer, ChangePanel, PanelTransitionDuration, false);
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

void UMainMenuUserWidget::ChangePanel(const EMainMenuButton Button)
{
	if (Button == EMainMenuButton::Settings)
	{
		UMenuUserWidget::Settings();
		UUserWidget::PlayAnimationForward(SettingsPanelAnimation);
	}
	else if (Button == EMainMenuButton::Back)
	{
		UMenuUserWidget::Back();
		UUserWidget::PlayAnimationForward(MainMenuPanelAnimation);
	}
}
