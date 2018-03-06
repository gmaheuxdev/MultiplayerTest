#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiplayerTestGameInstance.generated.h"

UCLASS()
class MULTIPLAYERTEST_API UMultiplayerTestGameInstance : public UGameInstance
{
	GENERATED_BODY()

//Member methods
public:
	
	UMultiplayerTestGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init();

private:

	//Console commands
	UFUNCTION(Exec)
	void Host();
	UFUNCTION(Exec)
	void JoinGame(const FString& instanceAdress);
};
