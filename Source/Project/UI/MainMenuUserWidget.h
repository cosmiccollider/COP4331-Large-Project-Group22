// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MenuUserWidget.h"
#include "MainMenuUserWidget.generated.h"

class UButton;

UENUM()
enum class EMainMenuButton : uint8
{
	Play,
	Settings,
	Back,
	Quit
};

/**
 * MainMenuUserWidget provides a user interface for use on the MainMenuMap
 */
UCLASS()
class PROJECT_API UMainMenuUserWidget : public UMenuUserWidget
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

protected:
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

	/**
	 * Called to trigger the transition animation and change the level
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void StartLevelTransition(const EMainMenuButton Button);
	
	/**
	 * Called to trigger the panel switch animation and change the panel
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void StartPanelTransition(const EMainMenuButton Button);

	/**
	 * Called to trigger a level change based on the button pressed
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void ChangeLevel(const EMainMenuButton Button);

	/**
	 * Called to trigger a panel change based on the button pressed
	 *
	 * @param	Button		specifies which button type was pressed
	 */
	UFUNCTION()
	void ChangePanel(const EMainMenuButton Button);

	/** Stores the settings panel animation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MainMenuPanelAnimation;
};
