// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevelScriptActor.generated.h"

class UMainMenuUserWidget;

/**
 * 
 */
UCLASS()
class PROJECT_API AMainMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMainMenuLevelScriptActor();

	// Main Menu User Widget
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<UMainMenuUserWidget> MainMenuClass;

	UPROPERTY()
		UMainMenuUserWidget* MainMenu;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
