#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MenuWidget.generated.h"


UCLASS()
class MULTIPLAYERTEST_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
//Getters and setters
public:
	void SetMainMenuInterface(IMainMenuInterface* newMainMenuInterface);

//Member variables
protected:

	IMainMenuInterface* m_MainMenuInterface; //Communicate with abstract interface instead of direct compile time coupling
	APlayerController* m_CachedPlayerController;

//Member methods
public:
	void DisplayMenu();
	void ExitMenu();

protected:
	virtual bool Initialize() override;
};
