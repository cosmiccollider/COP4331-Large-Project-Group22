// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DefaultActor.h"
#include "UObject/ConstructorHelpers.h"

ADefaultActor::ADefaultActor()
{
 	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create StaticMeshComponent and attach the RootComponent to it
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	// Set the default static mesh to a cube
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(CubeFinder.Object);
		StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

void ADefaultActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefaultActor::BeginPlay()
{
	Super::BeginPlay();
}
