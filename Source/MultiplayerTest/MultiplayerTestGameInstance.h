#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenuInterface.h"
#include "MainMenuWidget.h"
#include "OnlineSessionInterface.h"
#include "MultiplayerTestGameInstance.generated.h"



UCLASS()
class MULTIPLAYERTEST_API UMultiplayerTestGameInstance : public UGameInstance, public IMainMenuInterface
{
	GENERATED_BODY()

//Member variables
private:
	
	//Reference to widgets
	TSubclassOf<UUserWidget> m_MainMenuClassRef;
	TSubclassOf<UUserWidget> m_PauseMenuClassRef;
	class UMainMenuWidget* m_MainMenuWidget;

	//Online SubSystem
	class IOnlineSubsystem* m_OnlineSubSystemInterface; //The subsystem itself(Steam,Null,etc)
	IOnlineSessionPtr m_OnlineSessionInterface; //Interface implemented by the subsystem
	class TSharedPtr<FOnlineSessionSearch> m_SessionSearch; //Because it needs to survive until the callback is called
	
	//Other
	FString m_ServerName;


//Member methods
protected:
	
	UMultiplayerTestGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init();

	void SetupOnlineSubsystem();

private:
		
	//Overridden interface methods
	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;
	UFUNCTION(BlueprintCallable)
	virtual void QuitGame() override;
	UFUNCTION(Exec)
	virtual void Host(FString serverName) override;
	UFUNCTION(Exec)
	virtual void JoinServer(uint32 serverIndexToJoin) override;
	UFUNCTION(BlueprintCallable)
	virtual void RefreshServerList() override;

	//Other
	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();
	UFUNCTION(BlueprintCallable)
	void LoadPauseMenu();
	void CreateSession();
	
	//Callbacks
	UFUNCTION()
	void OnSessionCreationCompleteCallback(FName currentSessionName, bool isSucces);
	UFUNCTION()
	void OnSessionDestructionCompleteCallback(FName currentSessionName, bool isSucces);
	UFUNCTION()
	void OnFindSessionsCompleteCallback(bool isSucces);

	void SetupFoundSessionData(const FOnlineSessionSearchResult &result, TArray<FServerFoundData> &foundServerDataArray);

	void OnJoinSessionCompleteCallback(FName sessionJoined, EOnJoinSessionCompleteResult::Type joinSessionResult);
};
