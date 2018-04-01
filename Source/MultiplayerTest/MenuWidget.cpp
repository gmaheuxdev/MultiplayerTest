#include "MenuWidget.h"

//Getters and setters
void UMenuWidget::SetMainMenuInterface(IMainMenuInterface* newMainMenuInterface)
{
	m_MainMenuInterface = newMainMenuInterface; //Menu talks to interface to remove compile time coupling
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
bool UMenuWidget::Initialize()
{
	if (Super::Initialize())
	{
		UWorld* currentWorld = GetWorld();
		if (currentWorld != nullptr)
		{
			m_CachedPlayerController = currentWorld->GetFirstPlayerController();
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void UMenuWidget::DisplayMenu()
{
	if (m_CachedPlayerController != nullptr)
	{
		FInputModeUIOnly inputModeToSet;
		inputModeToSet.SetWidgetToFocus(this->TakeWidget());
		inputModeToSet.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		m_CachedPlayerController->bShowMouseCursor = true;
		m_CachedPlayerController->SetInputMode(inputModeToSet);
		this->AddToViewport();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void UMenuWidget::ExitMenu()
{
	if (m_CachedPlayerController != nullptr)
	{
		FInputModeGameOnly inputModeToSet;
		m_CachedPlayerController->bShowMouseCursor = false;
		m_CachedPlayerController->SetInputMode(inputModeToSet);
		this->RemoveFromViewport();
	}
}
