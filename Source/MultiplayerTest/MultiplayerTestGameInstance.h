#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainMenuInterface.h"
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
	virtual void JoinServer(const FString& instanceAdress) override;

	//Other
	UFUNCTION(BlueprintCallable)
	void LoadMenu();
	UFUNCTION(BlueprintCallable)
	void LoadPauseMenu();
};
