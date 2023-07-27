// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CreditsUserWidget.h"
#include "Components/Button.h"
#include "Levels/DefaultLevelScriptActor.h"
#include "UI/OverlayUserWidget.h"

void UCreditsUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitButton)
	{
		// Bind ExitButton to Exit() function and initialize it as hidden
		ExitButton->OnClicked.AddDynamic(this, &UCreditsUserWidget::Exit);
		ExitButton->SetVisibility(ESlateVisibility::Hidden);

		// Call ExitAnimation to play after 5 seconds to reveal ExitButton
		FTimerHandle AnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(AnimationTimer, this, &UCreditsUserWidget::PlayExitAnimation, 5.0f, false);
	}

	// Bind SecretButton to Secret() function
	if (SecretButton)
	{
		SecretButton->OnClicked.AddDynamic(this, &UCreditsUserWidget::Secret);
	}
}

void UCreditsUserWidget::Exit()
{
	if (ADefaultLevelScriptActor* DefaultLevelScript = Cast<ADefaultLevelScriptActor>(GetWorld()->GetLevelScriptActor()))
	{
		DefaultLevelScript->EndLevel(nullptr, ENextLevel::Default);
	}
}

void UCreditsUserWidget::Secret()
{
	if (ADefaultLevelScriptActor* DefaultLevelScript = Cast<ADefaultLevelScriptActor>(GetWorld()->GetLevelScriptActor()))
	{
		DefaultLevelScript->EndLevel(nullptr, ENextLevel::Secret);
	}
}

void UCreditsUserWidget::PlayExitAnimation()
{
	ExitButton->SetVisibility(ESlateVisibility::Visible);
	
	if (ExitAnimation)
	{
		UUserWidget::PlayAnimationForward(ExitAnimation);
	}
}
