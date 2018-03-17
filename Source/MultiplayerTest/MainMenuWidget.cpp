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
	//Subscribe buttons callback to OnClick Event
	if (m_HostButton != nullptr){m_HostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostServerClickCallback);}
	if (m_OpenJoinMenuButton != nullptr){m_OpenJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OpenJoinMenuClickCallback);}
	if (m_BackToMainMenuButton != nullptr){m_BackToMainMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ReturnToMainMenuClickCallback);}
	if (m_JoinServerMenuButton != nullptr){	m_JoinServerMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinServerClickCallback);}
	if (m_QuitGameMenuButton != nullptr) { m_QuitGameMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitGameButtonCallback);}
}

///////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::HostServerClickCallback()
{
	if (m_MainMenuInterface != nullptr)
	{
		m_MainMenuInterface->Host();
		ExitMenu();
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
void UMainMenuWidget::PopulateServerList(TArray<FString> foundServersList)
{
	if (m_ServerListScrollBox != nullptr && m_ServerRowClassRef != nullptr)
	{
		m_ServerListScrollBox->ClearChildren();
		UWorld* currentWorld = GetWorld();
		if (currentWorld != nullptr)
		{
			int32 serverIndexToAssign = 0;
			for (const FString& currentServerName : foundServersList)
			{
				UServerRowWidget*  serverRowToAdd = CreateWidget<UServerRowWidget>(currentWorld, m_ServerRowClassRef);
				if (serverRowToAdd != nullptr && m_ServerListScrollBox != nullptr)
				{
					serverRowToAdd->SetServerIndex(serverIndexToAssign);
					serverRowToAdd->m_ServerNameText->SetText(FText::FromString(currentServerName));
					serverRowToAdd->SetParentMainMenu(this);
					m_ServerListScrollBox->AddChild(serverRowToAdd);
					serverIndexToAssign++;
				}
			}
		}
	}
}
