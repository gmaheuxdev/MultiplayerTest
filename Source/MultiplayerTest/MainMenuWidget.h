#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuInterface.h"
#include "MenuWidget.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class MULTIPLAYERTEST_API UMainMenuWidget : public UMenuWidget
{
	GENERATED_BODY()


//Getters and setters
public:

int32 GetSelectedServerIndex(); //pas sur de si c'est vraiment clair comme maniere
void SetSelectedServerIndex(int32 newIndex);

private:
//Member variables

	//Warning: Buttons in interface need to be the same name as variable for binding to work
	UPROPERTY(meta = (BindWidget))
	class UButton* m_HostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_OpenJoinMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_BackToMainMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_JoinServerMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_QuitGameMenuButton;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* m_ServerListScrollBox;
	
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* m_MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UWidget* m_JoinMenuWidget;
	UPROPERTY(meta = (BindWidget))
	class UWidget* m_MainMenuWidget;

	TSubclassOf<UUserWidget> m_ServerRowClassRef;

	int32 m_SelectedServerIndex; //Ouain?? en member variable??

protected:
	
	UMainMenuWidget();
	virtual bool Initialize() override;
	void SetupMenuCallbacks();

	//OnClick Callbacks
	UFUNCTION()
	void HostServerClickCallback();
	UFUNCTION()
	void OpenJoinMenuClickCallback();
	UFUNCTION()
	void ReturnToMainMenuClickCallback();
	UFUNCTION()
	void JoinServerClickCallback();
	UFUNCTION()
	void QuitGameButtonCallback();

public:
	UFUNCTION(BlueprintCallable)
	void PopulateServerList(TArray<FString> foundServersList);
};
