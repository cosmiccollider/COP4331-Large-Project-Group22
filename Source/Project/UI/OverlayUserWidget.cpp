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
