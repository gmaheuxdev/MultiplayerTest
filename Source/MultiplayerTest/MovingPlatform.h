#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class MULTIPLAYERTEST_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
public:

	void SetMovementSpeed(int newMovementSpeed);
	int GetMovementSpeed();
	AMovingPlatform();

protected:
	virtual void Tick(float DeltaSeconds) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Platform Properties")
	int m_MovementSpeed;

};
