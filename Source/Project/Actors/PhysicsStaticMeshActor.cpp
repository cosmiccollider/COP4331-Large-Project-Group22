// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PhysicsStaticMeshActor.h"

void APhysicsStaticMeshActor::EnableGravity()
{
	GetStaticMeshComponent()->SetEnableGravity(true);
}

void APhysicsStaticMeshActor::DisableGravity()
{
	GetStaticMeshComponent()->SetEnableGravity(false);
}
