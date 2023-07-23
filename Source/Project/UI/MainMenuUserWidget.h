// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenuUserWidget.h"
#include "MainMenuUserWidget.generated.h"

class UButton;

/**
 * MainMenuUserWidget provides a user interface for use on the MainMenuMap
 */
UCLASS()
class PROJECT_API UMainMenuUserWidget : public USettingsMenuUserWidget
{
	GENERATED_BODY()
	
protected:
	/** Called when the game starts or when spawned */
	virtual void NativeConstruct() override;

	/** Stores the PlayButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* PlayButton;
	
	/** Called to start a new game on the first level */
	UFUNCTION()
	void Play();

	/** Stores the QuitButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* QuitButton;

	/** Called to quit the game to the desktop */
	UFUNCTION()
	void Quit();
};
