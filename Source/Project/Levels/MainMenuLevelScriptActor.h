// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevelScriptActor.generated.h"

class UMainMenuUserWidget;

/**
 * MainMenuLevelScriptActor provides the level specific capabilities of the MainMenuMap.
 */
UCLASS()
class PROJECT_API AMainMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	AMainMenuLevelScriptActor();

	/** Stores the class of MainMenuUserWidget */
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UMainMenuUserWidget> MainMenuClass;

	/** Stores the MainMenu for this level */
	UPROPERTY()
	UMainMenuUserWidget* MainMenu;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
};
