// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/BGPlayerState.h"

#include "Core/Lobby/BGLobbyGameStateBase.h"
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
	}
}
