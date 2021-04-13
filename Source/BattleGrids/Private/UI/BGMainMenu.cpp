// © 2021 Matthew Barham. All Rights Reserved.

#include "UI/BGMainMenu.h"
#include "Core/BGTypes.h"
#include "UI/BGServerRow.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"

void UBGMainMenu::SetActiveWidget(int const& Index)
{
	MenuSwitcher->SetActiveWidgetIndex(Index);
}

void UBGMainMenu::SetServerList(TArray<FBGServerData> ServerNames)
{
	auto const World = this->GetWorld();
	if (!ensure(World)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (FBGServerData const& ServerData : ServerNames)
	{
		UBGServerRow* ServerRow = CreateWidget<UBGServerRow>(World, ServerRowWidgetClass);
		if (!ensure(ServerRow)) return;

		ServerRow->SetServerData(ServerData);
		ServerRow->Setup(this, i);
		++i;

		ServerList->AddChild(ServerRow);
	}
}

void UBGMainMenu::SelectIndex(uint32 const Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

UBGMainMenu::UBGMainMenu(FObjectInitializer const& ObjectInitializer)
{
	if (ServerRowWidgetClass->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *ServerRowWidgetClass->GetName());
	}
}

bool UBGMainMenu::Initialize()
{
	bool const bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	// Initialize Main Menu widgets and bind Callbacks
	if (!ensure(HostButton)) return false;
	HostButton->OnClicked.AddDynamic(this, &UBGMainMenu::HostServer);

	if (!ensure(JoinButton)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UBGMainMenu::OpenJoinMenu);

	if (!ensure(OptionsButton)) return false;
	OptionsButton->OnClicked.AddDynamic(this, &UBGMainMenu::OpenOptionsMenu);

	if (!ensure(QuitButton)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UBGMainMenu::QuitPressed);

	// Initialize Join Menu widgets and bind Callbacks
	if (!ensure(CancelJoinMenuButton)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UBGMainMenu::OpenMainMenu);

	if (!ensure(RefreshListJoinMenuButton)) return false;
	RefreshListJoinMenuButton->OnClicked.AddDynamic(this, &UBGMainMenu::RefreshServerList);

	if (!ensure(ConfirmJoinMenuButton)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UBGMainMenu::JoinServer);

	// Initialize Options Menu widgets and bind Callbacks
	if (!ensure(CancelOptionsMenuButton)) return false;
	CancelOptionsMenuButton->OnClicked.AddDynamic(this, &UBGMainMenu::OpenMainMenu);

	if (!ensure(ConfirmOptionsMenuButton)) return false;
	ConfirmOptionsMenuButton->OnClicked.AddDynamic(this, &UBGMainMenu::ConfirmOptions);

	if (!ensure(PlayerNameEntry)) return false;
	PlayerNameEntry->OnTextChanged.AddDynamic(this, &UBGMainMenu::SetOptionsButtonEnabledState);

	if (PlayerNameEntry->GetText().IsEmpty())
	{
		ConfirmOptionsMenuButton->SetIsEnabled(false);
	}

	return true;
}

void UBGMainMenu::UpdateChildren()
{
	for (int32 i{}; i < ServerList->GetChildrenCount(); ++i)
	{
		auto Row = Cast<UBGServerRow>(ServerList->GetChildAt(i));
		if (Row)
		{
			Row->SetRowIsSelected(SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

void UBGMainMenu::HostServer()
{
	if (MenuInterface)
	{
		if (!ServerNameTextBox->GetText().IsEmpty())
		{
			MenuInterface->Host(ServerNameTextBox->GetText().ToString());
			ServerNameTextBox->SetText(FText());
		}
		else
		{
			MenuInterface->Host("Default");
		}
	}
}

void UBGMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		if (auto const ServerRow = Cast<UBGServerRow>(ServerList->GetChildAt(SelectedIndex.GetValue())))
		{
			UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue())
			MenuInterface->Join(SelectedIndex.GetValue(), ServerRow->GetServerData());
			MenuSwitcher->SetActiveWidget(MainMenu);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index Not Set"))
	}
}

void UBGMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(JoinMenu)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

	RefreshServerList();
}

void UBGMainMenu::RefreshServerList()
{
	if (MenuInterface)
	{
		ServerList->ClearChildren();
		MenuInterface->RefreshServerList();
	}
}

void UBGMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(MainMenu)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UBGMainMenu::OpenOptionsMenu()
{
	if (!ensure(MenuSwitcher)) return;
	if (!ensure(OptionsMenu)) return;
	MenuSwitcher->SetActiveWidget(OptionsMenu);
}

void UBGMainMenu::ConfirmOptions()
{
	if (!PlayerNameEntry->GetText().IsEmpty())
	{
		FBGPlayerInfo NewPlayerInfo;
		NewPlayerInfo.PlayerName = PlayerNameEntry->GetText();

		MenuInterface->Save(NewPlayerInfo);
		PlayerNameEntry->SetText(FText());
		OpenMainMenu();
	}
}

void UBGMainMenu::SetOptionsButtonEnabledState(FText const& InText)
{
	if (InText.IsEmpty())
	{
		ConfirmOptionsMenuButton->SetIsEnabled(false);
	}
	else
	{
		ConfirmOptionsMenuButton->SetIsEnabled(true);
	}
}

void UBGMainMenu::QuitPressed()
{
	auto const World = GetWorld();
	if (!ensure(World)) return;

	auto PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	PlayerController->ConsoleCommand("quit");
}
