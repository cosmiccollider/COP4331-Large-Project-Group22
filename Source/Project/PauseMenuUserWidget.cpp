// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuUserWidget.h"
#include "Components/Button.h"
#include "FirstPersonCharacter.h"
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

// Pause Menu Functions
void UPauseMenuUserWidget::Resume()
{
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AFirstPersonCharacter* PCharacter = Cast<AFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	PCharacter->PauseMenu->RemoveFromParent();
	PCharacter->PauseMenu = nullptr;
	FInputModeGameOnly InputMode;
	PController->SetInputMode(InputMode);
	PController->SetShowMouseCursor(false);
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	PCharacter->inPauseMenu = false;
}

void UPauseMenuUserWidget::MainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuMap");
}

void UPauseMenuUserWidget::Desktop()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
