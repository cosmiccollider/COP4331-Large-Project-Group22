// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SafeUserWidget.h"
#include "Actors/DefaultCharacter.h"
#include "Actors/SafeActor.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void USafeUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Play the animation to open the safe user interface
	UUserWidget::PlayAnimationForward(SafePanelAnimation);

	// Bind BackButton to Back() function
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USafeUserWidget::Back);
	}

	// Bind Digit1IncButton to Digit1Inc() function
	if (Digit1IncButton)
	{
		Digit1IncButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit1Inc);
	}

	// Bind Digit1DecButton to Digit1Dec() function
	if (Digit1DecButton)
	{
		Digit1DecButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit1Dec);
	}

	// Bind Digit2IncButton to Digit2Inc() function
	if (Digit2IncButton)
	{
		Digit2IncButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit2Inc);
	}

	// Bind Digit2DecButton to Digit2Dec() function
	if (Digit2DecButton)
	{
		Digit2DecButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit2Dec);
	}

	// Bind Digit3IncButton to Digit3Inc() function
	if (Digit3IncButton)
	{
		Digit3IncButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit3Inc);
	}

	// Bind Digit3DecButton to Digit3Dec() function
	if (Digit3DecButton)
	{
		Digit3DecButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit3Dec);
	}

	// Bind Digit4IncButton to Digit4Inc() function
	if (Digit4IncButton)
	{
		Digit4IncButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit4Inc);
	}

	// Bind Digit4DecButton to Digit4Dec() function
	if (Digit4DecButton)
	{
		Digit4DecButton->OnClicked.AddDynamic(this, &USafeUserWidget::Digit4Dec);
	}
}

void USafeUserWidget::Back()
{
	// Set game to unpaused
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// Play the animation to close the safe user interface
	UUserWidget::PlayAnimationReverse(SafePanelAnimation);
	float PanelTransitionDuration = SafePanelAnimation->GetEndTime();

	// Set a timer to exit the safe after the transition animation completes
	FTimerHandle ExitTimer;
	GetWorld()->GetTimerManager().SetTimer(ExitTimer, this, &USafeUserWidget::ExitSafe, PanelTransitionDuration, false);
}

void USafeUserWidget::ExitSafe()
{
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ADefaultCharacter* PCharacter = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Remove Safe UI from the screen
	PCharacter->SafeUI->RemoveFromParent();
	PCharacter->SafeUI = nullptr;

	// Set input mode to Game and hide the mouse cursor
	FInputModeGameOnly InputMode;
	PController->SetInputMode(InputMode);
	PController->SetShowMouseCursor(false);
}

void USafeUserWidget::Digit1Inc()
{
	UpdateDigitText(Digit1Text, ESafeDigitUpdate::Increment);
}

void USafeUserWidget::Digit1Dec()
{
	UpdateDigitText(Digit1Text, ESafeDigitUpdate::Decrement);
}

void USafeUserWidget::Digit2Inc()
{
	UpdateDigitText(Digit2Text, ESafeDigitUpdate::Increment);
}

void USafeUserWidget::Digit2Dec()
{
	UpdateDigitText(Digit2Text, ESafeDigitUpdate::Decrement);
}

void USafeUserWidget::Digit3Inc()
{
	UpdateDigitText(Digit3Text, ESafeDigitUpdate::Increment);
}

void USafeUserWidget::Digit3Dec()
{
	UpdateDigitText(Digit3Text, ESafeDigitUpdate::Decrement);
}

void USafeUserWidget::Digit4Inc()
{
	UpdateDigitText(Digit4Text, ESafeDigitUpdate::Increment);
}

void USafeUserWidget::Digit4Dec()
{
	UpdateDigitText(Digit4Text, ESafeDigitUpdate::Decrement);
}

void USafeUserWidget::UpdateDigitText(UTextBlock* const Digit, const ESafeDigitUpdate Update)
{
	uint8 CurrentNum = FCString::Atoi(*Digit->GetText().ToString());
	uint8 NewNum;

	if (Update == ESafeDigitUpdate::Increment)
	{
		NewNum = (CurrentNum + 1) % 10;
	}
	else if (Update == ESafeDigitUpdate::Decrement)
	{
		NewNum = (CurrentNum == 0) ? 9 : (CurrentNum - 1) % 10;
	}
	
	Digit->SetText(FText::Format(NSLOCTEXT("Safe", "SafeNum", "{0}"), NewNum));

	if (OwningSafe && IsCombinationCorrect())
	{
		OwningSafe->OpenSafe();
		Back();
	}
}

bool USafeUserWidget::IsCombinationCorrect()
{
	uint8 Digit1 = FCString::Atoi(*Digit1Text->GetText().ToString());
	uint8 Digit2 = FCString::Atoi(*Digit2Text->GetText().ToString());
	uint8 Digit3 = FCString::Atoi(*Digit3Text->GetText().ToString());
	uint8 Digit4 = FCString::Atoi(*Digit4Text->GetText().ToString());

	FString CurrentCombination = FText::Format(NSLOCTEXT("Safe", "SafeCombination", "{0}{1}{2}{3}"), Digit1, Digit2, Digit3, Digit4).ToString();
	return OwningSafe->GetCorrectCombination().Equals(CurrentCombination);
}
