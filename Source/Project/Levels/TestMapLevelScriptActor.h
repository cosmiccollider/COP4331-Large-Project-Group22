// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Levels/DefaultLevelScriptActor.h"
#include "TestMapLevelScriptActor.generated.h"

class APhysicsStaticMeshActor;

/**
 * 
 */
UCLASS()
class PROJECT_API ATestMapLevelScriptActor : public ADefaultLevelScriptActor
{
	GENERATED_BODY()
	
public:
	ATestMapLevelScriptActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		APhysicsStaticMeshActor* TestCube;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		APhysicsStaticMeshActor* TestBox;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
