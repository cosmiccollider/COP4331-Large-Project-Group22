// Fill out your copyright notice in the Description page of Project Settings.


#include "Levels/CreditsLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "UI/CreditsUserWidget.h"

ACreditsLevelScriptActor::ACreditsLevelScriptActor()
{
	// Set CreditsClass to CreditsWidgetBlueprint class
	static ConstructorHelpers::FClassFinder<UCreditsUserWidget> CreditsFinder(TEXT("/Game/UI/WBP_Credits.WBP_Credits_C"));
	if (CreditsFinder.Succeeded())
	{
		CreditsClass = CreditsFinder.Class;
	}
}

void ACreditsLevelScriptActor::BeginPlay()
{
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Create Credits and add it to the screen
	if (CreditsClass)
	{
		Credits = CreateWidget<UCreditsUserWidget>(PController, CreditsClass);
		Credits->AddToViewport();

		// Set input mode to UI, then show the mouse cursor
		FInputModeUIOnly InputMode;
		PController->SetInputMode(InputMode);
		PController->SetShowMouseCursor(true);
	}
}
