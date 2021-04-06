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
 * 
 */
UCLASS()
class BATTLEGRIDS_API UBGMainMenu : public UBGMenuWidget
{
	GENERATED_BODY()

public:

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
	void OpenMainMenu();

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
	TSubclassOf<UUserWidget> ServerRowWidgetClass;

	// unexposed variables
	TOptional<uint32> SelectedIndex;
};
