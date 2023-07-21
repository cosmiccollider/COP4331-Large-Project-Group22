// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayUserWidget.generated.h"

class UBorder;
class UImage;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECT_API UOverlayUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Overrides
	virtual void NativeConstruct() override;

	// FPS
	UFUNCTION()
	void ShowFPS(bool bVisible);

	UFUNCTION()
	void SetFPSText();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* FPSText;

	// Narrative
	UFUNCTION()
	void ShowNarrative(bool bVisible);

	UFUNCTION()
	void SetNarrativeText(FText Text);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UBorder* NarrativeBorder;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* NarrativeText;
	
	// Reticle
	UFUNCTION()
	void ShowReticle(bool bVisible);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* ReticleImage;
};
