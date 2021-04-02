// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/BGMenuWidget.h"
#include "BGLobbyMenu.generated.h"

class ABGPlayerState;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API UBGLobbyMenu : public UBGMenuWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	void UpdateLobbyInfo(FBGServerData const ServerData) const;

private:
	// Callback Functions
	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void ToggleReady();

	UFUNCTION()
	void OpenSettingsMenu();

	UFUNCTION()
	void OpenColorMenu();

	UFUNCTION()
	void LeaveLobby();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* ServerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* CurrentPlayerCount;

	// Buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* StartGameButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* ReadyButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* GameSettingsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* SelectColorButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* LeaveLobbyButton;
};
