#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

///////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class ActivationConditionEnum : uint8
{
	ActivationConditionEnum_Amount,
	ActivationConditionEnum_None
};
////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class MULTIPLAYERTEST_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
private:
	
	//Member variables
	UPROPERTY(EditAnywhere, Category = "Platform Properties")
	int m_MovementSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Platform Properties", Meta = (MakeEditWidget = true))
	FVector m_MovementTargetLocation;
	
	UPROPERTY(EditAnywhere, Category = "Platform Properties")
	TArray<ActivationConditionEnum> m_ActivationConditionsArray;
	
	UPROPERTY(EditAnywhere, Category = "Platform Properties")
	int m_ActivatedTriggersNeededAmount;
	
	//Cached variables
	FVector m_CachedGlobalStartLocation;
	FVector m_CachedGlobalMovementTargetLocation;

	//Other
	int m_AmountTriggersActivated;
	bool m_IsActivated;

	//Member methods
public:

	AMovingPlatform();
	void ValidateActivationConditions();
	void AddActivatedTrigger();
	void RemoveActivatedTrigger();

protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void UpdatePosition(float DeltaSeconds);
	bool CheckTriggerAmountCondition();
};
