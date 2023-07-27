// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsUserWidget.generated.h"

class UButton;
class UWidgetAnimation;

/**
 * CreditsUserWidget provides a HUD for the game to deliver the credits to the player
 */
UCLASS()
class PROJECT_API UCreditsUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/** Called when the game starts or when spawned */
	virtual void NativeConstruct() override;

protected:
	/** Stores the ExitButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ExitButton;

	/** Called to exit the credits to the main menu */
	UFUNCTION()
	void Exit();
	
	/** Stores the SecretButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* SecretButton;

	/** Called to exit the credits to the main menu */
	UFUNCTION()
	void Secret();

	/** Called to play the exit animation */
	UFUNCTION()
	void PlayExitAnimation();

	/** Stores the exit animation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ExitAnimation;
};
