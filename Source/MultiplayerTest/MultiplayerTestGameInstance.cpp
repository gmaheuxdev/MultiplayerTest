#include "MultiplayerTestGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "MainMenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.h"
#include "PauseMenuWidget.h"

UMultiplayerTestGameInstance::UMultiplayerTestGameInstance(const FObjectInitializer & ObjectInitializer)
{
	//Setup widget references
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPclass(TEXT("/Game/MenuUI/MainMenuWidget"));
	ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPclass(TEXT("/Game/MenuUI/PauseMenuWidget"));
	m_MainMenuClassRef = MainMenuBPclass.Class;
	m_PauseMenuClassRef = PauseMenuBPclass.Class;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::Init()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::LoadMenu()
{
	if (m_MainMenuClassRef != nullptr)
	{
		UMainMenuWidget* mainMenuWidget = CreateWidget<UMainMenuWidget>(this, m_MainMenuClassRef);
		
		if(mainMenuWidget != nullptr)
		{
			mainMenuWidget->SetMainMenuInterface(this); //Tell menu I am "implementing" the "interface"
			mainMenuWidget->SetupMenu();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::LoadPauseMenu()
{
	if (m_PauseMenuClassRef != nullptr)
	{
		UPauseMenuWidget* pauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, m_PauseMenuClassRef);
		pauseMenuWidget->SetupMenu();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::QuitGame()
{
	APlayerController* currentPlayerController = GetFirstLocalPlayerController();
	if (currentPlayerController != nullptr)
	{
		currentPlayerController->ConsoleCommand("quit");
	}
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
void UMultiplayerTestGameInstance::JoinServer(const FString & instanceAdress)
{
	APlayerController* currentPlayerController = GetFirstLocalPlayerController();
		
	if (currentPlayerController != nullptr)
	{
		currentPlayerController->ClientTravel(instanceAdress, ETravelType::TRAVEL_Absolute);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::LoadMainMenu()
{
	APlayerController* currentPlayerController = GetFirstLocalPlayerController();

	if (currentPlayerController != nullptr)
	{
		currentPlayerController->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenuMap",ETravelType::TRAVEL_Absolute);
	}
}