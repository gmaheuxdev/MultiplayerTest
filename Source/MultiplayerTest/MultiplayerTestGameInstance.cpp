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

//Preprocessor directives
const static FName SESSION_NAME = TEXT("TestSession");
const static int MAX_SEARCH_RESULTS = 200000;
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerNameSettingsKey");

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
	SetupOnlineSubsystem();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::SetupOnlineSubsystem()
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
	m_SessionSearch = MakeShareable(new FOnlineSessionSearch()); //needs to stay alive for asynchronous call

	//Setup session search and launch it
	if (m_SessionSearch.IsValid())
	{
		m_SessionSearch->bIsLanQuery = false;
		m_SessionSearch->MaxSearchResults = MAX_SEARCH_RESULTS;
		m_SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		UE_LOG(LogTemp, Warning, TEXT("Now looking for a session"));
		m_OnlineSessionInterface->FindSessions(0, m_SessionSearch.ToSharedRef());
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
			m_MainMenuWidget->SetMainMenuInterface(this); //Tells menu I am "implementing" the "interface"
			m_MainMenuWidget->DisplayMenu();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::LoadPauseMenu()
{
	if (m_PauseMenuClassRef != nullptr)
	{
		UPauseMenuWidget* pauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, m_PauseMenuClassRef);
		pauseMenuWidget->DisplayMenu();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::CreateSession()
{
	//Setup session and launch creation
	FOnlineSessionSettings currentSessionSettings;
	currentSessionSettings.bIsLANMatch = false;
	currentSessionSettings.NumPublicConnections = 2;
	currentSessionSettings.bShouldAdvertise = true;
	currentSessionSettings.bUsesPresence = true; //needed to create an internet session
	currentSessionSettings.Set(SERVER_NAME_SETTINGS_KEY,m_ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
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
void UMultiplayerTestGameInstance::Host(FString serverName)
{
	if (m_OnlineSessionInterface.IsValid())
	{
		//Destroy here if exists and create new session in callback
		if (m_OnlineSessionInterface->GetNamedSession(SESSION_NAME) != nullptr)
		{
			m_OnlineSessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			m_ServerName = serverName;
			CreateSession();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::JoinServer(uint32 serverIndexToJoin)
{
	if (m_OnlineSessionInterface.IsValid() && m_SessionSearch.IsValid())
	{
		m_OnlineSessionInterface->JoinSession(0, SESSION_NAME, m_SessionSearch->SearchResults[serverIndexToJoin]);
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::OnSessionCreationCompleteCallback(FName currentSessionName, bool isSucces)
{
	UWorld* currentWorld = GetWorld();

	if (currentWorld != nullptr && isSucces)
	{
		currentWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
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
		TArray<FServerFoundData> foundServerDataArray;
		for (const FOnlineSessionSearchResult& result : m_SessionSearch->SearchResults)
		{
			if (result.IsValid())
			{
				SetupFoundSessionData(result, foundServerDataArray);
			}
		}

		m_MainMenuWidget->PopulateServerList(foundServerDataArray);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMultiplayerTestGameInstance::SetupFoundSessionData(const FOnlineSessionSearchResult &result, TArray<FServerFoundData> &foundServerDataArray)
{
	//Setup server found data and add it to an array of data for all found servers
	FServerFoundData currentSessionData;
	currentSessionData.MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
	currentSessionData.HostUserName = result.Session.OwningUserName;
	currentSessionData.AmountPlayersConnected = currentSessionData.MaxPlayers - result.Session.NumOpenPublicConnections;

	FString serverName;

	if (result.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, serverName))
	{
		currentSessionData.ServerName = serverName;
	}

	foundServerDataArray.Add(currentSessionData);
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

