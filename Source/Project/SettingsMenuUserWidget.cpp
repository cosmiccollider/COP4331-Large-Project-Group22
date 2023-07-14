// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenuUserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

USettingsMenuUserWidget::USettingsMenuUserWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> FPSFinder(TEXT("/Game/UI/WBP_FPS.WBP_FPS_C"));
	if (FPSFinder.Succeeded()) FPSClass = FPSFinder.Class;
}

// Override Native Construct
void USettingsMenuUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Settings_Button) Settings_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::Settings);
	if (Back_Button) Back_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::Back);
	if (Window_Mode_Windowed_Button) Window_Mode_Windowed_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetWindowed);
	if (Window_Mode_Fullscreen_Button) Window_Mode_Fullscreen_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetFullscreen);
	if (Resolution_1280x720_Button) Resolution_1280x720_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetResolution1280x720);
	if (Resolution_1920x1080_Button) Resolution_1920x1080_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetResolution1920x1080);
	if (Quality_Low_Button) Quality_Low_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityLow);
	if (Quality_Medium_Button) Quality_Medium_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityMedium);
	if (Quality_High_Button) Quality_High_Button->OnClicked.AddDynamic(this, &USettingsMenuUserWidget::SetQualityHigh);
	if (FPS_Check_Box) FPS_Check_Box->OnCheckStateChanged.AddDynamic(this, &USettingsMenuUserWidget::ShowFPS);
}

// Settings Function
void USettingsMenuUserWidget::Settings()
{
	Widget_Switcher->SetActiveWidgetIndex(1);
}

// Back Function
void USettingsMenuUserWidget::Back()
{
	Widget_Switcher->SetActiveWidgetIndex(0);
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

// Show FPS Function
void USettingsMenuUserWidget::ShowFPS(bool bIsChecked)
{
	if (bIsChecked && FPSClass)
	{
		FPS = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), FPSClass);
		FPS->AddToViewport();
	}
	else if (FPS)
	{
		FPS->RemoveFromParent();
		FPS = nullptr;
	}
}
