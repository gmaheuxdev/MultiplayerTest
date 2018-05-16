#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MenuWidget.h"
#include "MainMenuWidget.generated.h"

USTRUCT()
struct FServerFoundData
{
	GENERATED_BODY()

	FString ServerName;
	FString HostUserName;
	uint16	AmountPlayersConnected;
	uint16  MaxPlayers;
};

///////////////////////////////////////////////////////////////////////////
UCLASS()
class MULTIPLAYERTEST_API UMainMenuWidget : public UMenuWidget
{
	GENERATED_BODY()


//Getters and setters
public:

int32 GetSelectedServerIndex(); //pas sur de si c'est vraiment clair comme maniere
void SetSelectedServerIndex(int32 newIndex);


//Member variables
private:

	//MENU VISUAL BINDINGS
	//Warning: Names in interface need to be the same name as variable for binding to work
	UPROPERTY(meta = (BindWidget))
	class UButton* m_OpenHostMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_OpenJoinMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_BackToMainMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_JoinServerMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_QuitGameMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_BeginHostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_CancelHostButton;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* m_ServerListScrollBox;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* m_ServerNameTextBox;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* m_MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UWidget* m_JoinMenuWidget;
	UPROPERTY(meta = (BindWidget))
	class UWidget* m_MainMenuWidget;
	UPROPERTY(meta = (BindWidget))
	class UWidget* m_HostMenuWidget;

	
	TSubclassOf<UUserWidget> m_ServerRowClassRef;
	int32 m_SelectedServerIndex; //Server selected when clicked in join server list


//Member methods
protected:
	
	UMainMenuWidget();
	virtual bool Initialize() override;
	void SetupMenuCallbacks();

	//Callbacks
	UFUNCTION()
	void BeginHostClickCallback();
	UFUNCTION()
	void CancelHostClickCallback();
	UFUNCTION()
	void OpenJoinMenuClickCallback();
	UFUNCTION()
	void OpenHostMenuClickCallback();
	UFUNCTION()
	void ReturnToMainMenuClickCallback();
	UFUNCTION()
	void JoinServerClickCallback();
	UFUNCTION()
	void QuitGameButtonCallback();

public:
	void PopulateServerList(TArray<FServerFoundData> foundServersList);

private:
	void SetupServerRow(int serverIndexToAssign, UWorld* currentWorld, const FServerFoundData& currentServerFound);

};
