// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingsMenuUserWidget.h"
#include "PauseMenuUserWidget.generated.h"

class UButton;

UENUM()
enum class EPauseMenuButton : uint8
{
	Resume,
	Restart,
	MainMenu,
	Desktop
};

/**
 * PauseMenuUserWidget provides a user interface for the player to use game systems while the game is paused
 */
UCLASS()
class PROJECT_API UPauseMenuUserWidget : public USettingsMenuUserWidget
{
	GENERATED_BODY()
	
protected:
	/** Called when the game starts or when spawned */
	virtual void NativeConstruct() override;

protected:
	/** Stores the ResumeButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ResumeButton;
	
	/** Called to resume play on the current level */
	UFUNCTION()
	void Resume();

	/** Stores the RestartButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* RestartButton;
	
	/** Called to restart the current level */
	UFUNCTION()
	void Restart();

	/** Stores the MainMenuButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* MainMenuButton;
	
	/** Called to quit the game to the main menu */
	UFUNCTION()
	void MainMenu();

	/** Stores the DesktopButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* DesktopButton;
	
	/** Called to quit the game to the desktop */
	UFUNCTION()
	void Desktop();

	/**
	 * Called to trigger the transition animation and change the level
	 * 
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void StartTransition(const EPauseMenuButton Button);
	
	/**
	 * Called to trigger a level change based on the button pressed
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void ChangeLevel(const EPauseMenuButton Button);
};
