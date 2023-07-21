// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenuUserWidget.h"
#include "Actors/DefaultCharacter.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Types/SlateEnums.h"
#include "UI/OverlayUserWidget.h"

#define LOCTEXT_NAMESPACE "FPS"

// Override Native Construct
void USettingsMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Set Buttons
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::Settings);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::Back);
	}

	if (WindowModeWindowedButton)
	{
		WindowModeWindowedButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetWindowed);
	}

	if (WindowModeFullscreenButton)
	{
		WindowModeFullscreenButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetFullscreen);
	}

	if (Resolution1280x720Button)
	{
		Resolution1280x720Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetResolution1280x720);
	}

	if (Resolution1920x1080Button)
	{
		Resolution1920x1080Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetResolution1920x1080);
	}

	if (QualityLowButton)
	{
		QualityLowButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityLow);
	}

	if (QualityMediumButton)
	{
		QualityMediumButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityMedium);
	}

	if (QualityHighButton)
	{
		QualityHighButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityHigh);
	}

	if (FPSCheckBox)
	{
		FPSCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingsMenuUserWidget::ShowFPS);
	}

	if (MaxFPSTextBox)
	{
		MaxFPSTextBox->OnTextChanged.AddDynamic(this, &USettingsMenuUserWidget::CheckMaxFPS);
	}

	if (MaxFPSTextBox)
	{
		MaxFPSTextBox->OnTextCommitted.AddDynamic(this, &USettingsMenuUserWidget::SetMaxFPS);
	}

	// Check that the player character and overlay exist
	// Then check if FPSCheckBox should already be checked upon construction
	if (ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (UOverlayUserWidget* Overlay = PC->GetOverlay())
		{
			if (Overlay->FPSText->GetVisibility() == ESlateVisibility::Visible)
			{
				FPSCheckBox->SetCheckedState(ECheckBoxState::Checked);
			}
		}
	}

	// Set the Max FPS text box based on the current max fps from the console
	IConsoleVariable* Console = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
	MaxFPSTextBox->SetText(FText::Format(LOCTEXT("FPS", "{FPS}"), Console->GetInt()));
}

// Settings Function
void USettingsMenuUserWidget::Settings()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

// Back Function
void USettingsMenuUserWidget::Back()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

// Window Mode Functions
void USettingsMenuUserWidget::SetWindowed()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFullscreenMode(EWindowMode::Windowed);
	Settings->ApplyResolutionSettings(false);
}

void USettingsMenuUserWidget::SetFullscreen()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	Settings->ApplyResolutionSettings(false);
}

// Resolution Functions
void USettingsMenuUserWidget::SetResolution(FIntPoint Resolution)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetScreenResolution(Resolution);
	Settings->ApplyResolutionSettings(false);
}

void USettingsMenuUserWidget::SetResolution1280x720()
{
	FIntPoint Resolution = FIntPoint(1280, 720);
	SetResolution(Resolution);
}

void USettingsMenuUserWidget::SetResolution1920x1080()
{
	FIntPoint Resolution = FIntPoint(1920, 1080);
	SetResolution(Resolution);
}

// Quality Functions
void USettingsMenuUserWidget::SetQuality(int32 Value)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetOverallScalabilityLevel(Value);
	Settings->ApplyNonResolutionSettings();
}

void USettingsMenuUserWidget::SetQualityLow()
{
	SetQuality(0);
}

void USettingsMenuUserWidget::SetQualityMedium()
{
	SetQuality(1);
}

void USettingsMenuUserWidget::SetQualityHigh()
{
	SetQuality(2);
}

// FPS Functions
void USettingsMenuUserWidget::ShowFPS(bool bIsChecked)
{
	ADefaultCharacter* PC = Cast<ADefaultCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (bIsChecked)
	{
		PC->GetOverlay()->ShowFPS(true);
	}
	else
	{
		PC->GetOverlay()->ShowFPS(false);
	}
}

void USettingsMenuUserWidget::CheckMaxFPS(const FText& Text)
{
	// If the most recent character entered was not a number, chop it off the text field
	if (!Text.IsNumeric())
	{
		MaxFPSTextBox->SetText(FText::FromString(Text.ToString().LeftChop(1)));
	}
}

void USettingsMenuUserWidget::SetMaxFPS(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// Get the current value in the text field and set it through the console
		int32 FPS = FCString::Atoi(*Text.ToString());
		IConsoleVariable* Console = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
		Console->Set(FPS);
	}
}
