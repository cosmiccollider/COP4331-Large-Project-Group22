// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/DefaultLevelScriptActor.h"
#include "CreditsLevelScriptActor.generated.h"

class UCreditsUserWidget;

/**
 * CreditsLevelScriptActor provides the level specific capabilities of the CreditsMap.
 */
UCLASS()
class PROJECT_API ACreditsLevelScriptActor : public ADefaultLevelScriptActor
{
	GENERATED_BODY()
	
public:
	ACreditsLevelScriptActor();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:
	/** Stores the class of UCreditsUserWidget */
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UCreditsUserWidget> CreditsClass;

	/** Stores the Credits for this level */
	UPROPERTY()
	UCreditsUserWidget* Credits;
};
