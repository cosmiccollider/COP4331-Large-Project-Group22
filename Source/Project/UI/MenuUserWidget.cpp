// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuUserWidget.h"
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

void UMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Don't bind SettingsButton and BackButton in this class
	// Since those delegate functions should be overriden in the derived class

	// Bind WindowModeWindowedButton to SetWindowed() function
	if (WindowModeWindowedButton)
	{
		WindowModeWindowedButton->OnClicked.AddDynamic(this, &UMenuUserWidget::SetWindowed);
	}

	// Bind WindowModeFullscreenButton to SetFullscreen() function
	if (WindowModeFullscreenButton)
	{
		WindowModeFullscreenButton->OnClicked.AddDynamic(this, &UMenuUserWidget::SetFullscreen);
	}

	// Bind Resolution1280x720Button to SetResolution1280x720() function
	if (Resolution1280x720Button)
	{
		Resolution1280x720Button->OnClicked.AddDynamic(this, &UMenuUserWidget::SetResolution1280x720);
	}

	// Bind Resolution1920x1080Button to SetResolution1920x1080() function
	if (Resolution1920x1080Button)
	{
		Resolution1920x1080Button->OnClicked.AddDynamic(this, &UMenuUserWidget::SetResolution1920x1080);
	}

	// Bind QualityLowButton to SetQualityLow() function
	if (QualityLowButton)
	{
		QualityLowButton->OnClicked.AddDynamic(this, &UMenuUserWidget::SetQualityLow);
	}

	// Bind QualityMediumButton to SetQualityMedium() function
	if (QualityMediumButton)
	{
		QualityMediumButton->OnClicked.AddDynamic(this, &UMenuUserWidget::SetQualityMedium);
	}

	// Bind QualityHighButton to SetQualityHigh() function
	if (QualityHighButton)
	{
		QualityHighButton->OnClicked.AddDynamic(this, &UMenuUserWidget::SetQualityHigh);
	}

	// Bind FPSCheckBox to ShowFPS() function
	if (FPSCheckBox)
	{
		FPSCheckBox->OnCheckStateChanged.AddDynamic(this, &UMenuUserWidget::ShowFPS);
	}

	// Bind MaxFPSTextBox to CheckMaxFPS() function on text changed
	if (MaxFPSTextBox)
	{
		MaxFPSTextBox->OnTextChanged.AddDynamic(this, &UMenuUserWidget::CheckMaxFPS);
	}

	// Bind MaxFPSTextBox to SetMaxFPS() function on text committed
	if (MaxFPSTextBox)
	{
		MaxFPSTextBox->OnTextCommitted.AddDynamic(this, &UMenuUserWidget::SetMaxFPS);
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

void UMenuUserWidget::Settings()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UMenuUserWidget::Back()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMenuUserWidget::SetWindowed()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFullscreenMode(EWindowMode::Windowed);
	Settings->ApplyResolutionSettings(false);
}

void UMenuUserWidget::SetFullscreen()
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	Settings->ApplyResolutionSettings(false);
}

void UMenuUserWidget::SetResolution(const FIntPoint Resolution)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetScreenResolution(Resolution);
	Settings->ApplyResolutionSettings(false);
}

void UMenuUserWidget::SetResolution1280x720()
{
	FIntPoint Resolution = FIntPoint(1280, 720);
	SetResolution(Resolution);
}

void UMenuUserWidget::SetResolution1920x1080()
{
	FIntPoint Resolution = FIntPoint(1920, 1080);
	SetResolution(Resolution);
}

void UMenuUserWidget::SetQuality(const int32 Value)
{
	UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings();
	Settings->SetOverallScalabilityLevel(Value);
	Settings->ApplyNonResolutionSettings();
}

void UMenuUserWidget::SetQualityLow()
{
	SetQuality(0);
}

void UMenuUserWidget::SetQualityMedium()
{
	SetQuality(1);
}

void UMenuUserWidget::SetQualityHigh()
{
	SetQuality(2);
}

void UMenuUserWidget::ShowFPS(const bool bIsChecked)
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

void UMenuUserWidget::CheckMaxFPS(const FText& Text)
{
	// If the most recent character entered was not a number, chop it off the text field
	if (!Text.IsNumeric())
	{
		MaxFPSTextBox->SetText(FText::FromString(Text.ToString().LeftChop(1)));
	}
}

void UMenuUserWidget::SetMaxFPS(const FText& Text, const ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		// Get the current value in the text field and set it through the console
		int32 FPS = FCString::Atoi(*Text.ToString());
		IConsoleVariable* Console = IConsoleManager::Get().FindConsoleVariable(TEXT("t.MaxFPS"));
		Console->Set(FPS);
	}
}
