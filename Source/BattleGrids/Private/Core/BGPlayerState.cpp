// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/BGPlayerState.h"

#include "Core/Lobby/BGLobbyGameStateBase.h"
#include "Core/Lobby/BGLobbyPlayerController.h"
#include "Engine/DemoNetDriver.h"

void ABGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGPlayerState, PlayerInfo)
}

void ABGPlayerState::SetPlayerInfo(FBGPlayerInfo const& InPlayerInfo)
{
	PlayerInfo = InPlayerInfo;
}

void ABGPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (auto LobbyGameState = GetWorld()->GetGameState<ABGLobbyGameStateBase>())
	{
		LobbyGameState->AddPlayerInfoToArray(PlayerInfo);
		// auto LobbyPC = Cast<ABGLobbyPlayerController>(GetGameInstance()->GetFirstLocalPlayerController());
		// if (LobbyPC)
		// {
		// 	LobbyPC->ServerRefreshLobbyPlayerList(LobbyGameState->GetConnectedPlayersInfo());
		// }
	}
}

void ABGPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	auto const BGPlayerState = Cast<ABGPlayerState>(PlayerState);

	if (BGPlayerState)
	{
		PlayerInfo = BGPlayerState->GetPlayerInfo();
	}
}

