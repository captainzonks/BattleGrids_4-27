// © 2021 Matthew Barham. All Rights Reserved.


#include "UI/BGInGamePlayerRow.h"

#include "Components/Button.h"
#include "Core/BGPlayerState.h"
#include "UI/BGInGamePlayerList.h"

void UBGInGamePlayerRow::Setup(UBGInGamePlayerList* InParent, uint32 const InIndex)
{
	Parent = InParent;
	Index = InIndex;

	if (!ensure(GameMasterPermissionsButton)) return;
	GameMasterPermissionsButton->OnClicked.AddDynamic(this, &UBGInGamePlayerRow::OnGameMasterPermissionsClicked);
}

void UBGInGamePlayerRow::SetPlayerInfo(FBGPlayerInfo const& InPlayerInfo)
{
	PlayerInfo = InPlayerInfo;
}

void UBGInGamePlayerRow::OnGameMasterPermissionsClicked()
{
	if (Parent)
	{
		Parent->SelectIndex(Index);
	}
}

bool UBGInGamePlayerRow::Initialize()
{
	bool const bSuccess = Super::Initialize();
	if (!bSuccess) return false;

	// Only show the Start button for the Host, and the Ready button for the Clients
	if (Parent->GetOwningPlayerState<ABGPlayerState>()->GetPlayerInfo().bGameMasterPermissions)
	{
		GameMasterPermissionsButton->SetVisibility(ESlateVisibility::Visible);
		GameMasterPermissionsButton->SetIsEnabled(true);
	}
	else
	{
		GameMasterPermissionsButton->SetVisibility(ESlateVisibility::Hidden);
		GameMasterPermissionsButton->SetIsEnabled(false);
	}

	return true;
}
