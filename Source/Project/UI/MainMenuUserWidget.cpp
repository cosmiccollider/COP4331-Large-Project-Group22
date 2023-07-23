// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
	UGameplayStatics::OpenLevel(GetWorld(), "TestMap");
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
}

void UMainMenuUserWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Quit, false);
}
