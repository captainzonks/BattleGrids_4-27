// © 2021 Matthew Barham. All Rights Reserved.

#include "Core/Lobby/BGLobbyGameModeBase.h"

ABGLobbyGameModeBase::ABGLobbyGameModeBase()
{
	bUseSeamlessTravel = true;
}

void ABGLobbyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	ConnectedPlayersInfo.Empty();
}