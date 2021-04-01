// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/Lobby/BGLobbyGameModeBase.h"

void ABGLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	NewlyConnectedPlayer = NewPlayer;
	AllConnectedPlayers.AddUnique(NewPlayer);

	
	
	// auto World = GetWorld();
	// if (!ensure(World)) return;
	//
	// bUseSeamlessTravel = true;
	// World->ServerTravel("/Game/BattleGrids/Levels/DefaultGameMap?Listen");
}

void ABGLobbyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	// --NumberOfPlayers;
}
