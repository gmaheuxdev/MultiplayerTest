#pragma once

#include "CoreMinimal.h"
#include "MainMenuInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMainMenuInterface : public UInterface
{
	GENERATED_BODY()
};

class MULTIPLAYERTEST_API IMainMenuInterface
{
	GENERATED_BODY()

public:

	//All the interface methods need to be implemented
	virtual void Host(FString serverName) = 0; 
	virtual void JoinServer(uint32 serverIndexToJoin) = 0;
	virtual void LoadMainMenu() = 0;
	virtual void QuitGame() = 0;
	virtual void RefreshServerList() = 0;
};
