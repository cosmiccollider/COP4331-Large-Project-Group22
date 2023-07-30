// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SafeUserWidget.generated.h"

class ASafeActor;
class UButton;
class UTextBlock;
class UWidgetAnimation;

UENUM()
enum class ESafeDigitUpdate : uint8
{
	Increment,
	Decrement
};

/**
 * SafeUserWidget provides a user interface for the player to input a combination to open a safe
 */
UCLASS()
class PROJECT_API USafeUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	/** Called when the game starts or when spawned */
	virtual void NativeConstruct() override;

public:
	/** Stores the OwningSafe for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ASafeActor* OwningSafe;

protected:
	/** Stores the BackButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* BackButton;

	/** Called to start the process of leaving the safe user interface */
	UFUNCTION()
	void Back();

	/** Called to exit the safe user interface */
	UFUNCTION()
	void ExitSafe();

	/** Stores the Digit1Text for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Digit1Text;

	/** Stores the Digit1IncButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit1IncButton;

	/** Called to increment digit 1 */
	UFUNCTION()
	void Digit1Inc();

	/** Stores the Digit1DecButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit1DecButton;

	/** Called to decrement digit 1 */
	UFUNCTION()
	void Digit1Dec();

	/** Stores the Digit2Text for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Digit2Text;

	/** Stores the Digit2IncrementButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit2IncButton;

	/** Called to increment digit 2 */
	UFUNCTION()
	void Digit2Inc();

	/** Stores the Digit2DecButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit2DecButton;

	/** Called to decrement digit 2 */
	UFUNCTION()
	void Digit2Dec();

	/** Stores the Digit3Text for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Digit3Text;

	/** Stores the Digit3IncButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit3IncButton;

	/** Called to increment digit 3 */
	UFUNCTION()
	void Digit3Inc();

	/** Stores the Digit3DecButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit3DecButton;

	/** Called to decrement digit 3 */
	UFUNCTION()
	void Digit3Dec();

	/** Stores the Digit4Text for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* Digit4Text;

	/** Stores the Digit4IncButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit4IncButton;

	/** Called to increment digit 4 */
	UFUNCTION()
	void Digit4Inc();

	/** Stores the Digit4DecButton for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* Digit4DecButton;

	/** Called to decrement digit 4 */
	UFUNCTION()
	void Digit4Dec();

	/**
	 * Called to update the text for a digit
	 * 
	 * @param	Digit		specifies the digit to set text for
	 * @param	Update		specifies whether to increment or decrement the digit
	 */
	UFUNCTION()
	void UpdateDigitText(UTextBlock* const Digit, const ESafeDigitUpdate Update);

	/** Called to check if the current combination is correct */
	UFUNCTION()
	bool IsCombinationCorrect();

	/** Stores the safe panel animation for this widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SafePanelAnimation;
};
