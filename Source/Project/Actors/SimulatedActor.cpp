// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SimulatedActor.h"

ASimulatedActor::ASimulatedActor()
{
	StaticMeshComponent->SetSimulatePhysics(true);
}

void ASimulatedActor::SetGravity(const bool bEnabled)
{
	GetStaticMeshComponent()->SetEnableGravity(bEnabled);
}
