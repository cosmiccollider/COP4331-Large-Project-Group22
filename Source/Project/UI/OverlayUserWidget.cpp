// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OverlayUserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Components/MultilineEditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UOverlayUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Disable all overlay elements by default
	ShowFPS(false);
	ShowReticle(false);
	ShowNarrative(false);
	ShowInsideTracker(false);
	ShowOutsideTracker(false);
	ShowSafeTracker(false);

	// Fade the screen from black
	PlayTransitionAnimation(EUMGSequencePlayMode::Reverse);
}

void UOverlayUserWidget::ShowFPS(const bool bVisible)
{
	// FPSTimer handles the interval at which the FPSTextBox will be updated
	FTimerHandle FPSTimer;

	if (FPSTextBox && bVisible)
	{
		FPSTextBox->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(FPSTimer, this, &UOverlayUserWidget::SetFPSText, 0.1f, true);
	}
	else if (FPSTextBox)
	{
		FPSTextBox->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(FPSTimer);
	}
}

void UOverlayUserWidget::SetFPSText()
{
	if (FPSTextBox)
	{
		// Get the time since the last frame and update the FPS with it
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		FPSTextBox->SetText(FText::Format(NSLOCTEXT("FPS", "FPSValue", "{0} FPS"), UKismetMathLibrary::FTrunc(1.0f / DeltaTime)));
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

void UOverlayUserWidget::RemoveNarrative()
{
	if (NarrativeAnimation)
	{
		UUserWidget::PlayAnimationReverse(NarrativeAnimation);

		// Create a timer for the tracker to be hidden after the animation is done
		float NarrativeDuration = NarrativeAnimation->GetEndTime();
		FTimerHandle NarrativeTimer;
		FTimerDelegate NarrativeHidden = FTimerDelegate::CreateUFunction(this, FName("ShowNarrative"), false);
		GetWorld()->GetTimerManager().SetTimer(NarrativeTimer, NarrativeHidden, NarrativeDuration, false);
	}
}

void UOverlayUserWidget::RevealNarrative()
{
	if (NarrativeAnimation)
	{
		ShowNarrative(true);
		UUserWidget::PlayAnimationForward(NarrativeAnimation);
	}
}

void UOverlayUserWidget::SetNarrativeText(const FText Text)
{
	if (NarrativeTextBox)
	{
		NarrativeTextBox->SetText(Text);
	}
}

void UOverlayUserWidget::ShowNarrative(const bool bVisible)
{
	if (NarrativeTextBox && bVisible)
	{
		NarrativeTextBox->SetVisibility(ESlateVisibility::Visible);
	}
	else if (NarrativeTextBox)
	{
		NarrativeTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayUserWidget::RemoveInsideTracker()
{
	if (InsideTrackerAnimation)
	{
		UUserWidget::PlayAnimationReverse(InsideTrackerAnimation);

		// Create a timer for the tracker to be hidden after the animation is done
		float InsideDuration = InsideTrackerAnimation->GetEndTime();
		FTimerHandle InsideTimer;
		FTimerDelegate InsideHidden = FTimerDelegate::CreateUFunction(this, FName("ShowInsideTracker"), false);
		GetWorld()->GetTimerManager().SetTimer(InsideTimer, InsideHidden, InsideDuration, false);
	}
}

void UOverlayUserWidget::RevealInsideTracker()
{
	if (InsideTrackerAnimation)
	{
		ShowInsideTracker(true);
		UUserWidget::PlayAnimationForward(InsideTrackerAnimation);
	}
}

void UOverlayUserWidget::ShowInsideTracker(const bool bVisible)
{
	if (InsideTextBox && bVisible)
	{
		InsideTextBox->SetVisibility(ESlateVisibility::Visible);
	}
	else if (InsideTextBox)
	{
		InsideTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayUserWidget::UpdateInsideTracker(const uint8 Remaining, const uint8 Max)
{
	if (InsideTextBox)
	{
		InsideTextBox->SetText(FText::Format(NSLOCTEXT("Inside", "InsideItems", "Inside Cleaned: {0}/{1}"), Remaining, Max));
	}
}

void UOverlayUserWidget::RemoveOutsideTracker()
{
	if (OutsideTrackerAnimation)
	{
		UUserWidget::PlayAnimationReverse(OutsideTrackerAnimation);

		// Create a timer for the tracker to be hidden after the animation is done
		float OutsideDuration = OutsideTrackerAnimation->GetEndTime();
		FTimerHandle OutsideTimer;
		FTimerDelegate OutsideHidden = FTimerDelegate::CreateUFunction(this, FName("ShowOutsideTracker"), false);
		GetWorld()->GetTimerManager().SetTimer(OutsideTimer, OutsideHidden, OutsideDuration, false);
	}
}

void UOverlayUserWidget::RevealOutsideTracker()
{
	if (OutsideTrackerAnimation)
	{
		ShowOutsideTracker(true);
		UUserWidget::PlayAnimationForward(OutsideTrackerAnimation);
	}
}

void UOverlayUserWidget::ShowOutsideTracker(const bool bVisible)
{
	if (OutsideTextBox && bVisible)
	{
		OutsideTextBox->SetVisibility(ESlateVisibility::Visible);
	}
	else if (OutsideTextBox)
	{
		OutsideTextBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UOverlayUserWidget::UpdateOutsideTracker(const uint8 Remaining, const uint8 Max)
{
	if (OutsideTextBox)
	{
		OutsideTextBox->SetText(FText::Format(NSLOCTEXT("Outside", "OutsideItems", "Outside Cleaned: {0}/{1}"), Remaining, Max));
	}
}

void UOverlayUserWidget::RemoveSafeTracker()
{
	if (SafeTrackerAnimation)
	{
		UUserWidget::PlayAnimationReverse(SafeTrackerAnimation);

		// Create a timer for the tracker to be hidden after the animation is done
		float SafeDuration = SafeTrackerAnimation->GetEndTime();
		FTimerHandle SafeTimer;
		FTimerDelegate SafeHidden = FTimerDelegate::CreateUFunction(this, FName("ShowSafeTracker"), false);
		GetWorld()->GetTimerManager().SetTimer(SafeTimer, SafeHidden, SafeDuration, false);
	}
}

void UOverlayUserWidget::RevealSafeTracker()
{
	if (SafeTrackerAnimation)
	{
		ShowSafeTracker(true);
		UUserWidget::PlayAnimationForward(SafeTrackerAnimation);
	}
}

void UOverlayUserWidget::ShowSafeTracker(const bool bVisible)
{
	if (SafeTextBox && bVisible)
	{
		SafeTextBox->SetVisibility(ESlateVisibility::Visible);
	}
	else if (SafeTextBox)
	{
		SafeTextBox->SetVisibility(ESlateVisibility::Hidden);
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
			float TransitionDuration = TransitionAnimation->GetEndTime();
			FTimerHandle TransitionTimer;
			FTimerDelegate TransitionHidden = FTimerDelegate::CreateUFunction(this, FName("ShowTransition"), false);
			GetWorld()->GetTimerManager().SetTimer(TransitionTimer, TransitionHidden, TransitionDuration, false);
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
