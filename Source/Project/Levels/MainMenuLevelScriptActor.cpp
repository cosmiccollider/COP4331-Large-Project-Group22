// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainMenuUserWidget.h"
#include "UObject/ConstructorHelpers.h"

AMainMenuLevelScriptActor::AMainMenuLevelScriptActor()
{
	// Set MainMenuClass to MainMenuWidgetBlueprint class
	static ConstructorHelpers::FClassFinder<UMainMenuUserWidget> MainMenuFinder(TEXT("/Game/UI/WBP_MainMenu.WBP_MainMenu_C"));
	if (MainMenuFinder.Succeeded())
	{
		MainMenuClass = MainMenuFinder.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset not found: '/Game/UI/WBP_MainMenu.WBP_MainMenu_C'"));
	}
}

void AMainMenuLevelScriptActor::BeginPlay()
{
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Create Main Menu and add it to the screen
	if (MainMenuClass)
	{
		MainMenu = CreateWidget<UMainMenuUserWidget>(PController, MainMenuClass);
		MainMenu->AddToViewport();

		// Set input mode to UI, then show the mouse cursor
		FInputModeUIOnly InputMode;
		PController->SetInputMode(InputMode);
		PController->SetShowMouseCursor(true);
	}
}
