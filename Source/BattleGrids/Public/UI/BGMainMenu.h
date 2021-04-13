// © 2021 Matthew Barham. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"

#include "UI/BGMenuWidget.h"
#include "BGMainMenu.generated.h"

struct FBGServerData;

class UEditableTextBox;
class UButton;
class UWidgetSwitcher;
class UPanelWidget;
class UBGServerRow;

/**
 * Main Menu widget for BattleGrids
 */
UCLASS()
class BATTLEGRIDS_API UBGMainMenu : public UBGMenuWidget
{
	GENERATED_BODY()

public:

	void SetActiveWidget(int const& Index);

	void SetServerList(TArray<FBGServerData> ServerNames);

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
	void RefreshServerList();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void OpenOptionsMenu();

	UFUNCTION()
	void ConfirmOptions();

	UFUNCTION()
	void SetOptionsButtonEnabledState(FText const& InText);

	UFUNCTION()
	void QuitPressed();

	// Main Menu Widgets
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UEditableTextBox* ServerNameTextBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UWidget* MainMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UPanelWidget* ServerList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UWidget* JoinMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UWidget* OptionsMenu;

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
	UButton* RefreshListJoinMenuButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* ConfirmJoinMenuButton;

	// Options Menu Widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UEditableTextBox* PlayerNameEntry;

	// Options Menu Buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* CancelOptionsMenuButton;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	UButton* ConfirmOptionsMenuButton;

	// Class References
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true),
		Category = "UBGMainMenu|Config")
	TSubclassOf<UUserWidget> ServerRowWidgetClass;

	// unexposed variables
	TOptional<uint32> SelectedIndex;
};
