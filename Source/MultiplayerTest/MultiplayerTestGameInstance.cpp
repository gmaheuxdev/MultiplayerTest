#include "MultiplayerTestGameInstance.h"
#include "Engine/Engine.h"

UMultiplayerTestGameInstance::UMultiplayerTestGameInstance(const FObjectInitializer & ObjectInitializer)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::Init()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::Host()
{
	UWorld* currentWorld = GetWorld();

	if (currentWorld != nullptr)
	{
		currentWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::JoinGame(const FString & instanceAdress)
{
	APlayerController* currentPlayerController = GetFirstLocalPlayerController();
		
	if (currentPlayerController != nullptr)
	{
		currentPlayerController->ClientTravel(instanceAdress, ETravelType::TRAVEL_Absolute);
	}
}
