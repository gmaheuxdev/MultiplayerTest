#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.h"
#include "PauseMenuWidget.generated.h"


UCLASS()
class MULTIPLAYERTEST_API UPauseMenuWidget : public UMenuWidget
{
	GENERATED_BODY()

//Member variables
private:
	
	//Menu buttons must have the same name as in the visual interface for binding to happen
	UPROPERTY(meta = (BindWidget))
	class UButton* m_CancelPauseBUtton;
	UPROPERTY(meta = (BindWidget))
	class UButton* m_QuitToMainMenuButton;

//Member methods

private:

	virtual bool Initialize() override;
	void SetupMenuCallBacks();
	UFUNCTION()
	void CancelPauseButtonCallback();
	UFUNCTION()
	void QuitToMainMenuButtonCallback();
};
