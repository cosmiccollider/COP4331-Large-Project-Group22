// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenuUserWidget.h"
#include "PauseMenuUserWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PROJECT_API UPauseMenuUserWidget : public USettingsMenuUserWidget
{
	GENERATED_BODY()
	
protected:
	// Overrides
	virtual void NativeConstruct() override;

	// Pause Menu Functions
	UFUNCTION()
		void Resume();
	
	UFUNCTION()
		void MainMenu();
	
	UFUNCTION()
		void Desktop();

	// Pause Menu Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Resume_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Main_Menu_Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UButton* Desktop_Button;
};
