// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OverlayUserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define LOCTEXT_NAMESPACE "FPS"

void UOverlayUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Disable all overlay elements by default
	ShowFPS(false);
	ShowNarrative(false);
	ShowReticle(false);

	// Fade the screen from black
	PlayTransitionAnimation(EUMGSequencePlayMode::Reverse);
}

void UOverlayUserWidget::ShowFPS(const bool bVisible)
{
	// FPSTimer handles the interval at which the FPSText will be updated
	FTimerHandle FPSTimer;

	if (FPSText && bVisible)
	{
		FPSText->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(FPSTimer, this, &UOverlayUserWidget::SetFPSText, 0.1f, true);
	}
	else if (FPSText)
	{
		FPSText->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(FPSTimer);
	}
}

void UOverlayUserWidget::SetFPSText()
{
	if (FPSText)
	{
		// Get the time since the last frame and update the FPS with it
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		FPSText->SetText(FText::Format(LOCTEXT("FPS", "{FPS} FPS"), UKismetMathLibrary::FTrunc(1.0f / DeltaTime)));
	}
}

void UOverlayUserWidget::ShowNarrative(const bool bVisible)
{
	if (NarrativeBorder && NarrativeText && bVisible)
	{
		NarrativeBorder->SetVisibility(ESlateVisibility::Visible);
		NarrativeText->SetVisibility(ESlateVisibility::Visible);
	}
	else if (NarrativeBorder && NarrativeText)
	{
		NarrativeBorder->SetVisibility(ESlateVisibility::Hidden);
		NarrativeText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayUserWidget::SetNarrativeText(const FText Text)
{
	if (NarrativeText)
	{
		NarrativeText->SetText(Text);
	}
}

void UOverlayUserWidget::ShowReticle(const bool bVisible)
{
	if (ReticleImage && bVisible)
	{
		ReticleImage->SetVisibility(ESlateVisibility::Visible);
	}
	else if (ReticleImage)
	{
		ReticleImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayUserWidget::SetReticleColor(const FColor Color)
{
	if (ReticleImage)
	{
		ReticleImage->SetColorAndOpacity(FLinearColor(Color));
	}
}

void UOverlayUserWidget::PlayTransitionAnimation(const EUMGSequencePlayMode::Type PlayMode)
{
	if (TransitionAnimation)
	{
		ShowTransition(true);

		if (PlayMode == EUMGSequencePlayMode::Forward)
		{
			UUserWidget::PlayAnimationForward(TransitionAnimation);
		}
		else if (PlayMode == EUMGSequencePlayMode::Reverse)
		{
			UUserWidget::PlayAnimationReverse(TransitionAnimation);

			// Create a timer for the transition image to be hidden after the animation is done
			FTimerHandle TransitionTimer;
			FTimerDelegate TransitionHidden = FTimerDelegate::CreateUFunction(this, FName("ShowTransition"), false);
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, TransitionHidden, 1.0f, false);
		}
	}
}

void UOverlayUserWidget::ShowTransition(const bool bVisible)
{
	if (TransitionImage && bVisible)
	{
		TransitionImage->SetVisibility(ESlateVisibility::Visible);
	}
	else if (TransitionImage)
	{
		TransitionImage->SetVisibility(ESlateVisibility::Hidden);
	}
}
