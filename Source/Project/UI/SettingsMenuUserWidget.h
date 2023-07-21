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
 * 
 */
UCLASS()
class PROJECT_API USettingsMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// Overrides
	virtual void NativeConstruct() override;
	
	// Settings Function
	UFUNCTION()
	void Settings();
	
	// Back Function
	UFUNCTION()
	void Back();

	// Window Mode Functions
	UFUNCTION()
	void SetWindowed();

	UFUNCTION()
	void SetFullscreen();

	// Resolution Functions
	UFUNCTION()
	void SetResolution(FIntPoint Resolution);
	
	UFUNCTION()
	void SetResolution1280x720();

	UFUNCTION()
	void SetResolution1920x1080();

	// Quality Fuctions
	UFUNCTION()
	void SetQuality(int32 Value);

	UFUNCTION()
	void SetQualityLow();

	UFUNCTION()
	void SetQualityMedium();

	UFUNCTION()
	void SetQualityHigh();
	
	// FPS Functions
	UFUNCTION()
	void ShowFPS(bool bIsChecked);

	UFUNCTION()
	void CheckMaxFPS(const FText& Text);

	UFUNCTION()
	void SetMaxFPS(const FText& Text, ETextCommit::Type CommitMethod);

	// Widget Switcher
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	// Settings Button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SettingsButton;

	// Back Button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* BackButton;

	// Window Mode Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* WindowModeWindowedButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* WindowModeFullscreenButton;

	// Resolution Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Resolution1280x720Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Resolution1920x1080Button;

	// Quality Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QualityLowButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QualityMediumButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QualityHighButton;

	// FPS Check Box
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UCheckBox* FPSCheckBox;

	// Max FPS Text Box
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableTextBox* MaxFPSTextBox;
};
