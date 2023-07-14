// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"

void ADefaultHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	DrawRect(FColor::White, ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f, 2, 2);
}
