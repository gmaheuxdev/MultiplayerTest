#pragma once

#include "CoreMinimal.h"
#include "MultiplayerTestGameMode.h"
#include "LobbyGameMode.generated.h"


UCLASS()
class MULTIPLAYERTEST_API ALobbyGameMode : public AMultiplayerTestGameMode
{
	GENERATED_BODY()


//Member variables
private:
	
	int m_LobbyPlayerAmount = 0;
	FTimerHandle m_GameStartTimerHandle;

//Member methods
public:

	virtual void PostLogin(APlayerController* newPlayer) override;
	virtual void Logout(AController* Exiting) override;
	void StartGame();

};
