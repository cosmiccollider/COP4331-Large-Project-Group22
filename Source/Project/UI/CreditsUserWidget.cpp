// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CreditsUserWidget.h"
#include "Actors/DefaultCharacter.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/OverlayUserWidget.h"

#define MAIN_MENU_MAP "MainMenuMap"

void UCreditsUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitButton)
	{
		// Bind ExitButton to ExitTransition() function and initialize it as hidden
		ExitButton->OnClicked.AddDynamic(this, &UCreditsUserWidget::ExitTransition);
		ExitButton->SetVisibility(ESlateVisibility::Hidden);

		// Call ExitAnimation to play after 5 seconds to reveal ExitButton
		FTimerHandle AnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(AnimationTimer, this, &UCreditsUserWidget::PlayExitAnimation, 5.0f, false);
	}
}

void UCreditsUserWidget::Exit()
{
	UGameplayStatics::OpenLevel(GetWorld(), MAIN_MENU_MAP);
}

void UCreditsUserWidget::ExitTransition()
{
	// Play the overlay transition animation
	if (ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		PC->StartOverlayTransition();
	}

	// Set a timer to change the map after the transition animation completes
	FTimerHandle TransitionTimer;
	GetWorld()->GetTimerManager().SetTimer(TransitionTimer, this, &UCreditsUserWidget::Exit, 1.0f, false);
}

void UCreditsUserWidget::PlayExitAnimation()
{
	ExitButton->SetVisibility(ESlateVisibility::Visible);
	
	if (ExitAnimation)
	{
		UUserWidget::PlayAnimationForward(ExitAnimation);
	}
}
