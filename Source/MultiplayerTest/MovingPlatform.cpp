#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
}

///////////////////////////////////////////////////////////////////////
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) //Server side only
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	//Cached
	cachedGlobalStartLocation = GetActorLocation();
	cachedGlobalMovementTargetLocation = GetTransform().TransformPosition(m_MovementTargetLocation);
}

///////////////////////////////////////////////////////////////////////
void AMovingPlatform::Tick(float DeltaSeconds)
{
	if (HasAuthority()) //Server side only
	{
		UpdatePosition(DeltaSeconds);
	} 
}

//////////////////////////////////////////////////////////////////////
void AMovingPlatform::UpdatePosition(float DeltaSeconds)
{
	float startToEndVectorSize = (cachedGlobalMovementTargetLocation - cachedGlobalStartLocation).Size();
	float currentVectorSize = (GetActorLocation() - cachedGlobalStartLocation).Size();

	if (currentVectorSize > startToEndVectorSize)
	{
		Swap(cachedGlobalStartLocation, cachedGlobalMovementTargetLocation); //Go back to original position
	}

	FVector directionVector = (cachedGlobalMovementTargetLocation - cachedGlobalStartLocation).GetSafeNormal();
	SetActorLocation(GetActorLocation() + directionVector * DeltaSeconds * m_MovementSpeed);
}

