#include "MultiplayerTestGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "MainMenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.h"
#include "PauseMenuWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"


//Macro for sessionName
const static FName SESSION_NAME = TEXT("TestSession");


UMultiplayerTestGameInstance::UMultiplayerTestGameInstance(const FObjectInitializer & ObjectInitializer)
{
	//Setup widget references
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPclass(TEXT("/Game/MenuUI/MainMenuWidget"));
	ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPclass(TEXT("/Game/MenuUI/PauseMenuWidget"));
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPclass(TEXT("/Game/MenuUI/ServerRowWidget"));
	m_MainMenuClassRef = MainMenuBPclass.Class;
	m_PauseMenuClassRef = PauseMenuBPclass.Class;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::Init()
{
	m_OnlineSubSystemInterface = IOnlineSubsystem::Get();
	if (m_OnlineSubSystemInterface != nullptr)
	{
		m_OnlineSessionInterface = m_OnlineSubSystemInterface->GetSessionInterface();
		
		if (m_OnlineSessionInterface.IsValid())
		{
			m_OnlineSessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerTestGameInstance::OnSessionDestructionCompleteCallback);
			m_OnlineSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerTestGameInstance::OnSessionCreationCompleteCallback);
			m_OnlineSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerTestGameInstance::OnFindSessionsCompleteCallback);
			m_OnlineSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMultiplayerTestGameInstance::OnJoinSessionCompleteCallback);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::RefreshServerList()
{
	m_SessionSearch = MakeShareable(new FOnlineSessionSearch());//CHECK CA!!!!!

	if (m_SessionSearch.IsValid())
	{
		m_SessionSearch->bIsLanQuery = true;

		m_OnlineSessionInterface->FindSessions(0, m_SessionSearch.ToSharedRef());///////CHECK CA
		UE_LOG(LogTemp, Warning, TEXT("Now looking for a session"));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::LoadMenuWidget()
{
	if (m_MainMenuClassRef != nullptr)
	{
		m_MainMenuWidget = CreateWidget<UMainMenuWidget>(this, m_MainMenuClassRef);
		
		if(m_MainMenuWidget != nullptr)
		{
			m_MainMenuWidget->SetMainMenuInterface(this); //Tell menu I am "implementing" the "interface"
			m_MainMenuWidget->SetupMenu();
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::CreateSession()
{
	FOnlineSessionSettings currentSessionSettings;
	currentSessionSettings.bIsLANMatch = true;
	currentSessionSettings.NumPublicConnections = 2;
	currentSessionSettings.bShouldAdvertise = true;

	m_OnlineSessionInterface->CreateSession(0, SESSION_NAME, currentSessionSettings);
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
	if (m_OnlineSessionInterface.IsValid())
	{
		//Destroy here and create new session in callback
		if (m_OnlineSessionInterface->GetNamedSession(SESSION_NAME) != nullptr)
		{
			m_OnlineSessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Found session interface")); 
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::JoinServer(uint32 serverIndexToJoin)
{
	if (m_OnlineSessionInterface.IsValid() && m_SessionSearch.IsValid())
	{
		m_OnlineSessionInterface->JoinSession(0, SESSION_NAME, m_SessionSearch->SearchResults[serverIndexToJoin]);

	}
	

	//APlayerController* currentPlayerController = GetFirstLocalPlayerController();
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::OnSessionCreationCompleteCallback(FName currentSessionName, bool isSucces)
{
	UWorld* currentWorld = GetWorld();

	if (currentWorld != nullptr && isSucces)
	{
		currentWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::OnSessionDestructionCompleteCallback(FName currentSessionName, bool isSucces)
{
	if (isSucces)
	{
		CreateSession();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::OnFindSessionsCompleteCallback(bool isSucces)
{
	if (isSucces && m_SessionSearch.IsValid())
	{
		TArray<FString> foundServerNamesArray;
		for (const FOnlineSessionSearchResult& result : m_SessionSearch->SearchResults)
		{
			if (result.IsValid())
			{
				foundServerNamesArray.Add(result.GetSessionIdStr());
			}
		}

		m_MainMenuWidget->PopulateServerList(foundServerNamesArray);
	} 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::OnJoinSessionCompleteCallback(FName sessionJoined, EOnJoinSessionCompleteResult::Type joinSessionResult)
{
	if (m_OnlineSessionInterface.IsValid() && m_SessionSearch.IsValid())
	{
		FString serverToJoinAdress;
		APlayerController* currentPlayer = GetFirstLocalPlayerController();

		if (m_OnlineSessionInterface->GetResolvedConnectString(sessionJoined, serverToJoinAdress) && currentPlayer != nullptr)
		{
			currentPlayer->ClientTravel(serverToJoinAdress, ETravelType::TRAVEL_Absolute);
		}
	}
}

