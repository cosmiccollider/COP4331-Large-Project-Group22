// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PhysicsStaticMeshActor.h"

void APhysicsStaticMeshActor::SetGravity(bool bEnabled)
{
	GetStaticMeshComponent()->SetEnableGravity(bEnabled);
}
