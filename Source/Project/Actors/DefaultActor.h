// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultActor.generated.h"

class UStaticMeshComponent;

/**
 * DefaultActor provides a base class derived from Actor which we can make more specific actors from.
 */
UCLASS()
class PROJECT_API ADefaultActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADefaultActor();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Stores the StaticMeshComponent for this actor */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	/** Returns the StaticMeshComponent of this actor */
	UFUNCTION()
	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
};
