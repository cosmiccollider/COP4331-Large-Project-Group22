// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/DoorActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

ADoorActor::ADoorActor()
{
	// Set the default static mesh to a door
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorFinder(TEXT("/Game/Props/SM_SolidDoor.SM_SolidDoor"));
	if (DoorFinder.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(DoorFinder.Object);
	}

	// Create a PhysicsConstraint, then attach it and constrain it to the StaticMeshComponent
	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComponent"));
	PhysicsConstraint->SetupAttachment(StaticMeshComponent);
	PhysicsConstraint->SetConstrainedComponents(StaticMeshComponent, FName(), nullptr, FName());

	// Set angular swing and twist so that the door only rotates in the x axis for 90 degrees
	PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 90);
	PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

	// Set the door to automatically swing closed after being opened
	PhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	PhysicsConstraint->SetAngularOrientationDrive(true, false);
}
