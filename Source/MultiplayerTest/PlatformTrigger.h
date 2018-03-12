#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformTrigger.generated.h"

UCLASS()
class MULTIPLAYERTEST_API APlatformTrigger : public AActor
{
	GENERATED_BODY()

//Member variables
private:	
	
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* m_PlatformTriggerVolume;
	
	UPROPERTY(EditAnywhere)
	TArray<class AMovingPlatform*> m_LinkedPlatformsArray;
	
//Member Methods
public:	

	APlatformTrigger();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
