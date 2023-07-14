// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Override Native Construct
void UMainMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Play_Button) Play_Button->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Play);
	if (Quit_Button) Quit_Button->OnClicked.AddDynamic(this, &UMainMenuUserWidget::Quit);
}

// Main Menu Functions
void UMainMenuUserWidget::Play()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TestMap");
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
}

void UMainMenuUserWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
