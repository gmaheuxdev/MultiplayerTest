#include "PauseMenuWidget.h"
#include "Components/Button.h"

bool UPauseMenuWidget::Initialize()
{
	if (Super::Initialize())
	{
		SetupMenuCallBacks();
	}

	return true;
}

/////////////////////////////////////////////////////////////
void UPauseMenuWidget::SetupMenuCallBacks()
{
	if (m_CancelPauseBUtton != nullptr){m_CancelPauseBUtton->OnClicked.AddDynamic(this, &UPauseMenuWidget::CancelPauseButtonCallback);}
	if (m_QuitToMainMenuButton != nullptr) {m_QuitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::QuitToMainMenuButtonCallback);}
}

////////////////////////////////////////////////////////////////////////
void UPauseMenuWidget::CancelPauseButtonCallback()
{
	ExitMenu();
}

////////////////////////////////////////////////////////////////
void UPauseMenuWidget::QuitToMainMenuButtonCallback()
{
	if (m_CachedPlayerController != nullptr)
	{
		m_CachedPlayerController->ClientTravel("/Game/ThirdPersonCPP/Maps/MainMenuMap", ETravelType::TRAVEL_Absolute);
	}
}
