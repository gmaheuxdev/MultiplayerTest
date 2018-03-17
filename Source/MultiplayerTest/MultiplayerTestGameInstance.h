#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenuInterface.h"
#include "OnlineSessionInterface.h"
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

	class IOnlineSubsystem* m_OnlineSubSystemInterface;
	IOnlineSessionPtr m_OnlineSessionInterface;
	class TSharedPtr<FOnlineSessionSearch> m_SessionSearch; //Because it needs to survive until the callback is called

//Member methods
public:
	
	UMultiplayerTestGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init();
private:
		
	//Overridden interface methods
	UFUNCTION(BlueprintCallable)
	virtual void LoadMainMenu() override;
	UFUNCTION(BlueprintCallable)
	void QuitGame() override;
	UFUNCTION(Exec)
	virtual void Host() override;
	UFUNCTION(Exec)
	virtual void JoinServer(uint32 serverIndexToJoin) override;
	UFUNCTION(BlueprintCallable)
	virtual void RefreshServerList() override;

	//Other
	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();
	UFUNCTION(BlueprintCallable)
	void LoadPauseMenu();
	UFUNCTION(BlueprintCallable)
	void CreateSession();
	
	//Callbacks
	UFUNCTION()
	void OnSessionCreationCompleteCallback(FName currentSessionName, bool isSucces);
	UFUNCTION()
	void OnSessionDestructionCompleteCallback(FName currentSessionName, bool isSucces);
	UFUNCTION()
	void OnFindSessionsCompleteCallback(bool isSucces);
	
	void OnJoinSessionCompleteCallback(FName sessionJoined, EOnJoinSessionCompleteResult::Type joinSessionResult);

};
