// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuUserWidget.generated.h"

class UButton;
class UCheckBox;
class UEditableTextBox;
class UWidgetSwitcher;

/**
 * SettingsMenuUserWidget provides a user interface for the player to modify game settings
 */
UCLASS()
class PROJECT_API USettingsMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/** Called when the game starts or when spawned */
	virtual void NativeConstruct() override;

protected:
	/** Stores the WidgetSwitcher for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	/** Stores the SettingsButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* SettingsButton;
	
	/** Called to navigate to the settings menu panel */
	UFUNCTION()
	void Settings();

	/** Stores the BackButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* BackButton;
	
	/** Called to navigate back to the previous panel */
	UFUNCTION()
	void Back();

	/** Stores the WindowModeWindowedButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* WindowModeWindowedButton;

	/** Stores the WindowModeFullscreenButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* WindowModeFullscreenButton;
	
	/** Called to set the game window to windowed mode */
	UFUNCTION()
	void SetWindowed();

	/** Called to set the game window to fullscreen mode */
	UFUNCTION()
	void SetFullscreen();

	/** Stores the Resolution1280x720Button for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Resolution1280x720Button;

	/** Stores the Resolution1920x1080Button for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Resolution1920x1080Button;
	
	/**
	 * Called to set the resolution of the game window
	 *
	 * @param	Resolution		specifies an FIntPoint that represents an (X, Y) resolution
	 */
	UFUNCTION()
	void SetResolution(const FIntPoint Resolution);
	
	/** Called to set the game resolution to 1280x720 */
	UFUNCTION()
	void SetResolution1280x720();

	/** Called to set the game resolution to 1920x1080 */
	UFUNCTION()
	void SetResolution1920x1080();

	/** Stores the QualityLowButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* QualityLowButton;

	/** Stores the QualityMediumButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* QualityMediumButton;

	/** Stores the QualityHighButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* QualityHighButton;

	/**
	 * Called to set the graphics quality of the game
	 *
	 * @param	Value		specifies an int32 that represents an overall quality preset
	 */
	UFUNCTION()
	void SetQuality(const int32 Value);

	/** Called to set the graphics quality to low */
	UFUNCTION()
	void SetQualityLow();

	/** Called to set the graphics quality to medium */
	UFUNCTION()
	void SetQualityMedium();

	/** Called to set the graphics quality to high */
	UFUNCTION()
	void SetQualityHigh();

	/** Stores the FPSCheckBox for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UCheckBox* FPSCheckBox;
	
	/**
	 * Called to show or hide the FPS counter for the OverlayUserWidget
	 *
	 * @param	bIsChecked		specify true to show FPS counter
	 */
	UFUNCTION()
	void ShowFPS(const bool bIsChecked);

	/** Stores the MaxFPSTextBox for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UEditableTextBox* MaxFPSTextBox;
	
	/** Called to determine valid values are entered into MaxFPSTextBox */
	UFUNCTION()
	void CheckMaxFPS(const FText& Text);

	/** Called to set the max FPS to whatever value is in MaxFPSTextBox */
	UFUNCTION()
	void SetMaxFPS(const FText& Text, const ETextCommit::Type CommitMethod);
};
