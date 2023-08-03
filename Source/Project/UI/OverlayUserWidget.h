// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverlayUserWidget.generated.h"

class UImage;
class UEditableTextBox;
class UMultiLineEditableTextBox;
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
	/** Stores the FPSTextBox for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* FPSTextBox;

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

	/** Stores the NarrativeTextBox for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UMultiLineEditableTextBox* NarrativeTextBox;

	/** Stores the NarrativeAnimation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NarrativeAnimation;

	/** Called to remove the narrative from the screen */
	UFUNCTION()
	void RemoveNarrative();

	/** Called to reveal the narrative to the screen */
	UFUNCTION()
	void RevealNarrative();

	/**
	 * Called to set the text of the narrative
	 *
	 * @param	Text	specifies the FText to change the text to
	 */
	UFUNCTION()
	void SetNarrativeText(const FText Text);

	/**
	 * Called to show or hide the narrative text
	 *
	 * @param	bVisible	specify true to show narrative text
	 */
	UFUNCTION()
	void ShowNarrative(const bool bVisible);

	/** Stores the InsideTextBox for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* InsideTextBox;

	/** Stores the InsideTrackerAnimation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InsideTrackerAnimation;

	/** Called to remove the inside tracker from the screen */
	UFUNCTION()
	void RemoveInsideTracker();

	/** Called to reveal the inside tracker to the screen */
	UFUNCTION()
	void RevealInsideTracker();

	/**
	 * Called to show or hide the inside tracker
	 *
	 * @param	bVisible	specify true to show inside tracker
	 */
	UFUNCTION()
	void ShowInsideTracker(const bool bVisible);

	/**
	 * Called to update the inside tracker
	 *
	 * @param	Remaining		specifies the amount of items remaining
	 * @param	Max				specifies the max amount of items
	 */
	UFUNCTION()
	void UpdateInsideTracker(const uint8 Remaining, const uint8 Max);

	/** Stores the OutsideTextBox for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* OutsideTextBox;

	/** Stores the OutsideTrackerAnimation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* OutsideTrackerAnimation;
	
	/** Called to remove the outside tracker from the screen */
	UFUNCTION()
	void RemoveOutsideTracker();

	/** Called to reveal the outside tracker to the screen */
	UFUNCTION()
	void RevealOutsideTracker();

	/**
	 * Called to show or hide the outside tracker
	 *
	 * @param	bVisible	specify true to show outside tracker
	 */
	UFUNCTION()
	void ShowOutsideTracker(const bool bVisible);

	/**
	 * Called to update the outside tracker
	 *
	 * @param	Remaining		specifies the amount of items remaining
	 * @param	Max				specifies the max amount of items
	 */
	UFUNCTION()
	void UpdateOutsideTracker(const uint8 Remaining, const uint8 Max);

	/** Stores the SafeTextBox for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UMultiLineEditableTextBox* SafeTextBox;

	/** Stores the SafeTrackerAnimation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SafeTrackerAnimation;

	/** Called to remove the safe tracker to the screen */
	UFUNCTION()
	void RemoveSafeTracker();

	/** Called to reveal the safe tracker to the screen */
	UFUNCTION()
	void RevealSafeTracker();

	/**
	 * Called to show or hide the safe tracker
	 *
	 * @param	bVisible	specify true to show safe tracker
	 */
	UFUNCTION()
	void ShowSafeTracker(const bool bVisible);

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

	/** Stores the TransitionAnimation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* TransitionAnimation;
};
