#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRowWidget.generated.h"

UCLASS()
class MULTIPLAYERTEST_API UServerRowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Getters and setters
	void SetServerIndex(int32 newIndex);
	void SetParentMainMenu(class UMainMenuWidget* newMainMenuParent);
	int32 GetServerIndex();

		
public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* m_ServerNameText;

private:
	int32 m_ServerIndex; //index in server list
	UPROPERTY(meta = (BindWidget))
	class UButton* m_ServerRowButton;
	class UMainMenuWidget* m_ParentMainMenu;

protected:

	virtual bool Initialize() override;
	UFUNCTION()
	void ServerRowButtonClickCallback();

};
