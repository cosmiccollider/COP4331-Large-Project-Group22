// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuUserWidget.h"
#include "PauseMenuUserWidget.generated.h"

class UButton;
class UWidgetAnimation;

UENUM()
enum class EPauseMenuButton : uint8
{
	Resume,
	Restart,
	Settings,
	MainMenu,
	Desktop,
	Back
};

/**
 * PauseMenuUserWidget provides a user interface for the player to use game systems while the game is paused
 */
UCLASS()
class PROJECT_API UPauseMenuUserWidget : public UMenuUserWidget
{
	GENERATED_BODY()
	
public:
	/** Called to navigate back to the pause menu panel */
	UFUNCTION()
	void Back() override;

protected:
	/** Called when the game starts or when spawned */
	virtual void NativeConstruct() override;

	/** Called to navigate to the settings menu panel */
	UFUNCTION()
	void Settings() override;

public:
	/** Called to resume play on the current level */
	UFUNCTION()
	void Resume();

protected:
	/** Stores the ResumeButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ResumeButton;

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
	void StartLevelTransition(const EPauseMenuButton Button);

	/**
	 * Called to trigger the panel switch animation and change the panel
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void StartPanelTransition(const EPauseMenuButton Button);
	
	/**
	 * Called to trigger a level change based on the button pressed
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void ChangeLevel(const EPauseMenuButton Button);

	/**
	 * Called to trigger a panel change based on the button pressed
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void ChangePanel(const EPauseMenuButton Button);

	/** Stores the pause panel animation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* PausePanelAnimation;

	/** Called to resume the game from a paused state */
	UFUNCTION()
	void ResumeGame();
};
