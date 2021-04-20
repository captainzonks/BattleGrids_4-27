// © 2021 Matthew Barham. All Rights Reserved.


#include "UI/BGMenuWidget.h"

void UBGMenuWidget::Setup()
{
	this->AddToViewport();

	auto const World = GetWorld();
	if (!ensure(World)) return;
	auto PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	this->SetOwningPlayer(PlayerController);

	// set input mode to Game & UI and show mouse cursor
	FInputModeGameAndUI InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void UBGMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	auto const World = GetWorld();
	if (!ensure(World)) return;
	auto PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController)) return;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
}

void UBGMenuWidget::SetMenuInterface(IBGMenuInterface* NewMenuInterface)
{
	MenuInterface = NewMenuInterface;
}