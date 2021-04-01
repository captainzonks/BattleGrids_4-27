// © 2021 Matthew Barham. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "UI/BGMenuWidget.h"
#include "BGMainMenu.generated.h"

class UButton;
class UWidgetSwitcher;
class UPanelWidget;
class UBGServerRow;

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API UBGMainMenu : public UBGMenuWidget
{
	GENERATED_BODY()

public:

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 const Index);

protected:
	UBGMainMenu(FObjectInitializer const& ObjectInitializer);

	virtual bool Initialize() override;

private:

	void UpdateChildren();

	// Callback Functions
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void QuitPressed();

	// Main Menu Widgets
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UWidget* MainMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UPanelWidget* ServerList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UWidget* JoinMenu;

	// Main Menu Buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* HostButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* JoinButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* OptionsButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* QuitButton;

	// Join Menu Buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* CancelJoinMenuButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* ConfirmJoinMenuButton;

	// Class References
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true),
		Category = "UBGMainMenu|Config")
	TSubclassOf<UUserWidget> ServerRowClass;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true),
	// 	Category = "UBGMainMenu|Config")
	// UBGServerRow* ServerRow;

	// unexposed variables
	TOptional<uint32> SelectedIndex;
};
