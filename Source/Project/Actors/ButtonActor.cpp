// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ButtonActor.h"

void AButtonActor::FlipSwitch(const bool bActive)
{
	if (bActive)
	{
		GetStaticMeshComponent()->SetWorldRotation(FRotator(0, 0, 30));
	}
	else
	{
		GetStaticMeshComponent()->SetWorldRotation(FRotator(0, 0, -30));
	}
}
