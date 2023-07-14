// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenuUserWidget.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMainMenuUserWidget : public USettingsMenuUserWidget
{
	GENERATED_BODY()
	
protected:
	// Overrides
	virtual void NativeConstruct() override;

	// Main Menu Functions
	UFUNCTION()
		void Play();

	UFUNCTION()
		void Quit();

	// Main Menu Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Play_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Quit_Button;
};
