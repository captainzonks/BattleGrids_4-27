// © 2021 Matthew Barham. All Rights Reserved.


#include "UI/BGInGameMenu.h"

#include "Components/Button.h"

bool UBGInGameMenu::Initialize()
{
	bool const bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	if (!ensure(ResumeButton)) return false;
	ResumeButton->OnClicked.AddDynamic(this, &UBGInGameMenu::ResumePressed);

	if (!ensure(LeaveGameButton)) return false;
	LeaveGameButton->OnClicked.AddDynamic(this, &UBGInGameMenu::LeaveGamePressed);

	if (!ensure(QuitToDesktopButton)) return false;
	QuitToDesktopButton->OnClicked.AddDynamic(this, &UBGInGameMenu::QuitToDesktopPressed);
	
	return true;
}

void UBGInGameMenu::ResumePressed()
{
	Teardown();
}

void UBGInGameMenu::LeaveGamePressed()
{
	if (MenuInterface)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}

void UBGInGameMenu::QuitToDesktopPressed()
{
	auto OwningPlayerController = GetOwningPlayer();
	if (OwningPlayerController)
	{
		OwningPlayerController->ConsoleCommand("quit");
	}
}
