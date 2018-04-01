#include "LobbyGameMode.h"
#include "TimerManager.h"

const static int TIME_BEFORE_START = 10;

void ALobbyGameMode::PostLogin(APlayerController * newPlayer)
{
	//Game begins X seconds after the last player connects
	Super::PostLogin(newPlayer);
	GetWorldTimerManager().SetTimer(m_GameStartTimerHandle,this,&ALobbyGameMode::StartGame,TIME_BEFORE_START);
}

/////////////////////////////////////////////////////////////////////
void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
}

/////////////////////////////////////////////////////////////////////
void ALobbyGameMode::StartGame()
{
	UWorld* currentWorld = GetWorld();
	if (currentWorld != nullptr)
	{
		bUseSeamlessTravel = true;
		currentWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

