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
#include "Types/SlateEnums.h"
#include "UI/OverlayUserWidget.h"

#define LOCTEXT_NAMESPACE "FPS"

void USettingsMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Bind SettingsButton to Settings() function
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::Settings);
	}

	// Bind BackButton to Back() function
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::Back);
	}

	// Bind WindowModeWindowedButton to SetWindowed() function
	if (WindowModeWindowedButton)
	{
		WindowModeWindowedButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetWindowed);
	}

	// Bind WindowModeFullscreenButton to SetFullscreen() function
	if (WindowModeFullscreenButton)
	{
		WindowModeFullscreenButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetFullscreen);
	}

	// Bind Resolution1280x720Button to SetResolution1280x720() function
	if (Resolution1280x720Button)
	{
		Resolution1280x720Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetResolution1280x720);
	}

	// Bind Resolution1920x1080Button to SetResolution1920x1080() function
	if (Resolution1920x1080Button)
	{
		Resolution1920x1080Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetResolution1920x1080);
	}

	// Bind QualityLowButton to SetQualityLow() function
	if (QualityLowButton)
	{
		QualityLowButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityLow);
	}

	// Bind QualityMediumButton to SetQualityMedium() function
	if (QualityMediumButton)
	{
		QualityMediumButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityMedium);
	}

	// Bind QualityHighButton to SetQualityHigh() function
	if (QualityHighButton)
	{
		QualityHighButton->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityHigh);
	}

	// Bind FPSCheckBox to ShowFPS() function
	if (FPSCheckBox)
	{
		FPSCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingsMenuUserWidget::ShowFPS);
	}

	// Bind MaxFPSTextBox to CheckMaxFPS() function on text changed
	if (MaxFPSTextBox)
	{
		MaxFPSTextBox->OnTextChanged.AddDynamic(this, &USettingsMenuUserWidget::CheckMaxFPS);
	}

	// Bind MaxFPSTextBox to SetMaxFPS() function on text committed
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

void USettingsMenuUserWidget::Settings()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void USettingsMenuUserWidget::Back()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

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

void USettingsMenuUserWidget::SetResolution(const FIntPoint Resolution)
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

void USettingsMenuUserWidget::SetQuality(const int32 Value)
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

void USettingsMenuUserWidget::ShowFPS(const bool bIsChecked)
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

void USettingsMenuUserWidget::SetMaxFPS(const FText& Text, const ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// Get the current value in the text field and set it through the console
		int32 FPS = FCString::Atoi(*Text.ToString());
		IConsoleVariable* Console = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
		Console->Set(FPS);
	}
}
