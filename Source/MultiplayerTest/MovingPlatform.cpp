#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	m_AmountTriggersActivated = 0;
	m_IsActivated = false;
}

////////////////////////////////////////////////////////////////////////
void AMovingPlatform::AddActivatedTrigger()
{
	m_AmountTriggersActivated++;
}

/////////////////////////////////////////////////////////////////////////
void AMovingPlatform::RemoveActivatedTrigger()
{
	if (m_AmountTriggersActivated > 0)
	{
		m_AmountTriggersActivated--;
	}
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
	m_CachedGlobalStartLocation = GetActorLocation();
	m_CachedGlobalMovementTargetLocation = GetTransform().TransformPosition(m_MovementTargetLocation);
}

///////////////////////////////////////////////////////////////////////
void AMovingPlatform::Tick(float DeltaSeconds)
{
	if (HasAuthority() && m_IsActivated) //Server side only
	{
		UpdatePosition(DeltaSeconds);
	} 
}

//////////////////////////////////////////////////////////////////////
void AMovingPlatform::UpdatePosition(float DeltaSeconds)
{
	float startToEndVectorSize = (m_CachedGlobalMovementTargetLocation - m_CachedGlobalStartLocation).Size();
	float currentVectorSize = (GetActorLocation() - m_CachedGlobalStartLocation).Size();

	if (currentVectorSize > startToEndVectorSize)
	{
		Swap(m_CachedGlobalStartLocation, m_CachedGlobalMovementTargetLocation); //Go back to original position
	}

	FVector directionVector = (m_CachedGlobalMovementTargetLocation - m_CachedGlobalStartLocation).GetSafeNormal();
	SetActorLocation(GetActorLocation() + directionVector * DeltaSeconds * m_MovementSpeed);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AMovingPlatform::ValidateActivationConditions()
{
	m_IsActivated = true;
	for (int i = 0; i < m_ActivationConditionsArray.Num(); i++)
	{
		switch (m_ActivationConditionsArray[i])
		{
			case ActivationConditionEnum::ActivationConditionEnum_Amount:
				if (!CheckTriggerAmountCondition())
				{
					m_IsActivated = false;
					return;
				}
	
			default: break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
bool AMovingPlatform::CheckTriggerAmountCondition()
{
	if (m_AmountTriggersActivated == m_ActivatedTriggersNeededAmount)
	{
		return true;
	}
	return false;
}
