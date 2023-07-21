// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OverlayUserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#define LOCTEXT_NAMESPACE "FPS"

// Override Native Construct
void UOverlayUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShowFPS(false);
	ShowNarrative(false);
	ShowReticle(false);
}

// FPS
void UOverlayUserWidget::ShowFPS(bool bVisible)
{
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
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FPSText->SetText(FText::Format(LOCTEXT("FPS", "{FPS} FPS"), UKismetMathLibrary::FTrunc(1.0f / DeltaTime)));
}

// Narrative
void UOverlayUserWidget::ShowNarrative(bool bVisible)
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

void UOverlayUserWidget::SetNarrativeText(FText Text)
{
	NarrativeText->SetText(Text);
}

// Reticle
void UOverlayUserWidget::ShowReticle(bool bVisible)
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
