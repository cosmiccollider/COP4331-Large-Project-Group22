// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayUserWidget.generated.h"

class UBorder;
class UImage;
class UTextBlock;
class UWidgetAnimation;

/**
 * OverlayUserWidget provides a HUD for the player character which delivers information
 */
UCLASS()
class PROJECT_API UOverlayUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Called when the game starts or when spawned */
	virtual void NativeConstruct() override;

public:
	/** Stores the FPSText for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* FPSText;

	/**
	 * Called to show or hide the FPS counter
	 *
	 * @param	bVisible	specify true to show FPS counter
	 */
	UFUNCTION()
	void ShowFPS(const bool bVisible);

	/** Called to update the text inside the FPS counter */
	UFUNCTION()
	void SetFPSText();

	/** Stores the NarrativeBorder for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UBorder* NarrativeBorder;
	
	/** Stores the NarrativeText for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* NarrativeText;

	/**
	 * Called to show or hide the narrative content
	 *
	 * @param	bVisible	specify true to show narrative content
	 */
	UFUNCTION()
	void ShowNarrative(const bool bVisible);

	/**
	 * Called to set the text content of NarrativeText
	 *
	 * @param	Text	specifies an FText to change the text content to
	 */
	UFUNCTION()
	void SetNarrativeText(const FText Text);
	
	/** Stores the ReticleImage for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* ReticleImage;

	/**
	 * Called to show or hide the reticle image
	 *
	 * @param	bVisible	specify true to show reticle image
	 */
	UFUNCTION()
	void ShowReticle(const bool bVisible);

	/**
	 * Called to set the color of the reticle
	 *
	 * @param	Color	specifies an FColor to change the reticle to
	 */
	UFUNCTION()
	void SetReticleColor(const FColor Color);
	
	/** Returns the ReticleImage for this widget */
	UFUNCTION()
	UImage* GetReticle() const { return ReticleImage; };

	/** Stores the TransitionImage for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* TransitionImage;

	/**
	 * Called to play the transition animation
	 *
	 * @param	PlayMode	specifies the playback mode
	 */
	UFUNCTION()
	void PlayTransitionAnimation(const EUMGSequencePlayMode::Type PlayMode);

	/**
	 * Called to show or hide the transition image
	 *
	 * @param	bVisible	specify true to show transition image
	 */
	UFUNCTION()
	void ShowTransition(const bool bVisible);

	/** Stores the transition animation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TransitionAnimation;
};
