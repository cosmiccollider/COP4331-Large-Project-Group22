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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset not found: '/Engine/BasicShapes/Cube.Cube'"));
	}
}

void ADefaultActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADefaultActor::BeginPlay()
{
	Super::BeginPlay();

	// Set the material to a dynamic instance of itself
	DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(0), this);
	StaticMeshComponent->SetMaterial(0, DynamicMaterial);
}

void ADefaultActor::SetBaseColor(const FLinearColor Color)
{
	DynamicMaterial->SetVectorParameterValue(TEXT("Base Color"), Color);
}

void ADefaultActor::SetGlow(const float Glow)
{
	DynamicMaterial->SetScalarParameterValue(TEXT("Glow"), Glow);
}

void ADefaultActor::StartBlinking(const float InRate, const float Glow)
{
	FTimerDelegate BlinkFunction = FTimerDelegate::CreateUFunction(this, FName("Blink"), Glow);
	GetWorld()->GetTimerManager().SetTimer(BlinkTimer, BlinkFunction, InRate, true);
}

void ADefaultActor::Blink(const float Glow)
{
	float CurrentGlow;
	DynamicMaterial->GetScalarParameterValue(TEXT("Glow"), CurrentGlow);

	if (CurrentGlow != 0.0f)
	{
		SetGlow(0.0f);
	}
	else
	{
		SetGlow(Glow);
	}
}

void ADefaultActor::StopBlinking()
{
	GetWorld()->GetTimerManager().ClearTimer(BlinkTimer);
}
