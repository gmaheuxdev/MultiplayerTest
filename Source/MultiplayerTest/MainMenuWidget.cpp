#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UObject/ConstructorHelpers.h"
#include "MultiplayerTestGameInstance.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "ServerRowWidget.h"

//Getters and setters
void UMainMenuWidget::SetSelectedServerIndex(int32 newIndex){m_SelectedServerIndex = newIndex;}
int32 UMainMenuWidget::GetSelectedServerIndex() {return m_SelectedServerIndex;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
UMainMenuWidget::UMainMenuWidget()
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPclass(TEXT("/Game/MenuUI/ServerRowWidget"));
	m_ServerRowClassRef = ServerRowBPclass.Class;
	m_SelectedServerIndex = -1; //-1 == no server selected
}

////////////////////////////////////////////////////////////////////////////////////////
bool UMainMenuWidget::Initialize()
{
	if (Super::Initialize())
	{
		SetupMenuCallbacks();
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::SetupMenuCallbacks()
{
	//Subscribe callbacks to events
	if (m_OpenHostMenuButton != nullptr){m_OpenHostMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenHostMenuClickCallback);}
	if (m_OpenJoinMenuButton != nullptr){m_OpenJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenJoinMenuClickCallback);}
	if (m_BackToMainMenuButton != nullptr){m_BackToMainMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToMainMenuClickCallback);}
	if (m_JoinServerMenuButton != nullptr){	m_JoinServerMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinServerClickCallback);}
	if (m_QuitGameMenuButton != nullptr) { m_QuitGameMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGameButtonCallback);}
	if (m_BeginHostButton != nullptr) { m_BeginHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::BeginHostClickCallback);}
	if (m_CancelHostButton != nullptr) { m_CancelHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::CancelHostClickCallback);}
}

///////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::BeginHostClickCallback()
{
	FString currentServerName = "Unnamed Server";
		
	if (m_MainMenuInterface != nullptr)
	{
		if (m_ServerNameTextBox != nullptr)
		{
			currentServerName = m_ServerNameTextBox->Text.ToString();
		}

		m_MainMenuInterface->Host(currentServerName);
		ExitMenu();
	}
}

//////////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::CancelHostClickCallback()
{
	if (m_MenuSwitcher != nullptr && m_MainMenuWidget != nullptr)
	{
		m_MenuSwitcher->SetActiveWidget(m_MainMenuWidget);
	}
}

///////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::OpenJoinMenuClickCallback()
{
	if (m_MenuSwitcher != nullptr && m_JoinMenuWidget != nullptr && m_MainMenuInterface != nullptr)
	{
		m_MenuSwitcher->SetActiveWidget(m_JoinMenuWidget);
		m_MainMenuInterface->RefreshServerList();
	}
}

////////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::OpenHostMenuClickCallback()
{
	if (m_MenuSwitcher != nullptr && m_MainMenuWidget != nullptr && m_HostMenuWidget != nullptr)
	{
		m_MenuSwitcher->SetActiveWidget(m_HostMenuWidget);
	}
}

//////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::ReturnToMainMenuClickCallback()
{
	if (m_MenuSwitcher != nullptr && m_MainMenuWidget != nullptr)
	{
		m_MenuSwitcher->SetActiveWidget(m_MainMenuWidget);
	}
}

///////////////////////////////////////////////////////////////////////
void UMainMenuWidget::JoinServerClickCallback()
{
	ExitMenu();
	m_MainMenuInterface->JoinServer(m_SelectedServerIndex);
}

/////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::QuitGameButtonCallback()
{
	if (m_QuitGameMenuButton != nullptr && m_MainMenuInterface != nullptr)
	{
		m_MainMenuInterface->QuitGame();
	}
}

//////////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::PopulateServerList(TArray<FServerFoundData> foundServersData)
{
	//Create and setup a new row for every server found
	if (m_ServerListScrollBox != nullptr && m_ServerRowClassRef != nullptr)
	{
		m_ServerListScrollBox->ClearChildren();
		UWorld* currentWorld = GetWorld();
		if (currentWorld != nullptr)
		{
			int32 serverIndexToAssign = 0;
			for (const FServerFoundData& currentServerFound : foundServersData)
			{
				SetupServerRow(serverIndexToAssign,currentWorld,currentServerFound);
				serverIndexToAssign++;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::SetupServerRow(int serverIndexToAssign, UWorld* currentWorld, const FServerFoundData& currentServerFound)
{
	UServerRowWidget*  serverRowToAdd = CreateWidget<UServerRowWidget>(currentWorld, m_ServerRowClassRef);
	if (serverRowToAdd != nullptr && m_ServerListScrollBox != nullptr)
	{
		serverRowToAdd->SetServerIndex(serverIndexToAssign);
		serverRowToAdd->m_ServerNameText->SetText(FText::FromString(currentServerFound.ServerName));
		serverRowToAdd->m_MaximumPlayersAmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), currentServerFound.MaxPlayers)));
		serverRowToAdd->m_playersConnectedAmountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), currentServerFound.AmountPlayersConnected)));
		serverRowToAdd->SetParentMainMenu(this);
		m_ServerListScrollBox->AddChild(serverRowToAdd);
	}
}
