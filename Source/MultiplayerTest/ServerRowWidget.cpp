#include "ServerRowWidget.h"
#include "MainMenuWidget.h"
#include "Components/Button.h"

//Getters and setters
void UServerRowWidget::SetServerIndex(int32 newIndex){m_ServerIndex = newIndex;}
void UServerRowWidget::SetParentMainMenu(UMainMenuWidget* newMainMenuParent){m_ParentMainMenu = newMainMenuParent;}
int32 UServerRowWidget::GetServerIndex(){return m_ServerIndex;}

bool UServerRowWidget::Initialize()
{
	if (Super::Initialize())
	{
		if (m_ServerRowButton != nullptr) { m_ServerRowButton->OnClicked.AddDynamic(this, &UServerRowWidget::ServerRowButtonClickCallback); }
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
void UServerRowWidget::ServerRowButtonClickCallback()
{
	if (m_ParentMainMenu != nullptr)
	{
		m_ParentMainMenu->SetSelectedServerIndex(m_ServerIndex);
	}
}
 