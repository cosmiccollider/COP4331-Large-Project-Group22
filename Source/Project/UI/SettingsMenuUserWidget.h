// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenuUserWidget.generated.h"

class UButton;
class UCheckBox;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class PROJECT_API USettingsMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USettingsMenuUserWidget(const FObjectInitializer& ObjectInitializer);

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
	
	// Show FPS Function
	UFUNCTION()
		void ShowFPS(bool bIsChecked);

	// Widget Switcher
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
		UWidgetSwitcher* Widget_Switcher;

	// Settings Button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Settings_Button;

	// Back Button
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Back_Button;

	// Window Mode Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Window_Mode_Windowed_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Window_Mode_Fullscreen_Button;

	// Resolution Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Resolution_1280x720_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Resolution_1920x1080_Button;

	// Quality Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Quality_Low_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Quality_Medium_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Quality_High_Button;

	// FPS Check Box
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UCheckBox* FPS_Check_Box;

	// FPS User Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> FPSClass;

	UPROPERTY()
		UUserWidget* FPS;
};
