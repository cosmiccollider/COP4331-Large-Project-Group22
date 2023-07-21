// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DefaultLevelScriptActor.generated.h"

class AActor;
class APhysicsStaticMeshActor;

/**
 * 
 */
UCLASS()
class PROJECT_API ADefaultLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ADefaultLevelScriptActor();

	// Called to determine if an object is inside a container
	UFUNCTION()
	bool IsObjectInContainer(APhysicsStaticMeshActor* Object, APhysicsStaticMeshActor* Container);

	// Called to set gravity for all physics actors in the level
	UFUNCTION()
	void SetGravity(bool bEnabled);

	// Actor arrays
	UPROPERTY()
	TArray<AActor*> ActorArray;

	UPROPERTY()
	TArray<APhysicsStaticMeshActor*> PhysicsActorArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
