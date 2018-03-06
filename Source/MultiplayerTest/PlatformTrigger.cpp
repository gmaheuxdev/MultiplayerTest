#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "MovingPlatform.h"

APlatformTrigger::APlatformTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
	m_PlatformTriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("PlatformTriggerVolume"));

	if (m_PlatformTriggerVolume != nullptr)
	{
		RootComponent = m_PlatformTriggerVolume;
	}
}

/////////////////////////////////////////////////////////////////////////
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	m_PlatformTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	m_PlatformTriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

/////////////////////////////////////////////////////////////////////////
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	for (int i = 0; i < m_LinkedPlatformsArray.Num(); i++)
	{
		if (m_LinkedPlatformsArray[i] != nullptr)
		{
			m_LinkedPlatformsArray[i]->AddActivatedTrigger();
			m_LinkedPlatformsArray[i]->ValidateActivationConditions();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	for (int i = 0; i < m_LinkedPlatformsArray.Num(); i++)
	{
		if (m_LinkedPlatformsArray[i] != nullptr)
		{
			m_LinkedPlatformsArray[i]->RemoveActivatedTrigger();
			m_LinkedPlatformsArray[i]->ValidateActivationConditions();
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("OVERLAP FINISHED"));
}



