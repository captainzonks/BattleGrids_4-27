// © 2021 Matthew Barham. All Rights Reserved.


#include "UI/BGConnectedPlayer.h"

#include "Components/Button.h"

bool UBGConnectedPlayer::Initialize()
{
	bool const bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	if (!ensure(ConnectedPlayerButton)) return false;

	return true;
}

void UBGConnectedPlayer::Setup(UBGLobbyMenu* InParent, uint32 const InIndex)
{
	Parent = InParent;
	Index = InIndex;

	UpdateButtonColor(PlayerInfo.bReady);
}

void UBGConnectedPlayer::UpdateButtonColor(bool const bReady) const
{
	bReady
		? ConnectedPlayerButton->SetBackgroundColor(FLinearColor::Green)
		: ConnectedPlayerButton->SetBackgroundColor(FLinearColor::Red);
}
