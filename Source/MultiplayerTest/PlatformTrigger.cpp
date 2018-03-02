#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"

// Sets default values
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
	UE_LOG(LogTemp, Warning, TEXT("OMG OMG SO OVERLLAPED"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP FINISHED"));
}

