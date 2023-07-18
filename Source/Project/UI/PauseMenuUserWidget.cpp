// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"
#include "Components/Button.h"
#include "Actors/DefaultCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Override Native Construct
void UPauseMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Resume_Button) Resume_Button->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Resume);
	if (Main_Menu_Button) Main_Menu_Button->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::MainMenu);
	if (Desktop_Button) Desktop_Button->OnClicked.AddDynamic(this, &UPauseMenuUserWidget::Desktop);
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

void UPauseMenuUserWidget::MainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap");
}

void UPauseMenuUserWidget::Desktop()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
