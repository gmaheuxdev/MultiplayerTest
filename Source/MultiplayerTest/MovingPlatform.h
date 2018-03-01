#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class MULTIPLAYERTEST_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
public:

	AMovingPlatform();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
private:
	
	//Member variables
	UPROPERTY(EditAnywhere, Category = "Platform Properties")
	int m_MovementSpeed;
	UPROPERTY(EditAnywhere, Category = "Platform Properties", Meta = (MakeEditWidget = true))
	FVector m_MovementTargetLocation;
	
	//Cached variables
	FVector cachedGlobalStartLocation;
	FVector cachedGlobalMovementTargetLocation;
};
