#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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
	if (m_MenuSwitcher != nullptr && m_JoinMenuWidget != nullptr)
	{
		m_MenuSwitcher->SetActiveWidget(m_JoinMenuWidget);
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
	if (m_IPAdressTextBox != nullptr && m_MainMenuInterface != nullptr)
	{	
		m_MainMenuInterface->JoinServer(m_IPAdressTextBox->GetText().ToString());
		ExitMenu();
	}
 }

/////////////////////////////////////////////////////////////////////////
void UMainMenuWidget::QuitGameButtonCallback()
{
	if (m_QuitGameMenuButton != nullptr && m_MainMenuInterface != nullptr)
	{
		m_MainMenuInterface->QuitGame();
	}
}
