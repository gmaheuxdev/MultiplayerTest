#include "MovingPlatform.h"

int AMovingPlatform::GetMovementSpeed(){return m_MovementSpeed;}

//////////////////////////////////////////////////////////////////////
AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
}

///////////////////////////////////////////////////////////////////////
void AMovingPlatform::Tick(float DeltaSeconds)
{
	if (HasAuthority()) //Server side only
	{
		SetActorLocation(GetActorLocation() + FVector(m_MovementSpeed * DeltaSeconds, 0, 0));

	}
}
