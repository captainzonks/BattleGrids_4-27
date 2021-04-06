// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/Lobby/BGLobbyGameModeBase.h"

#include "Core/BGPlayerState.h"
#include "Core/Lobby/BGLobbyGameStateBase.h"
#include "Core/Lobby/BGLobbyPlayerController.h"
#include "UI/BGLobbyMenu.h"


void ABGLobbyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	while (!LobbyGameState)
	{
		LobbyGameState = GetGameState<ABGLobbyGameStateBase>();
	}
}

void ABGLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ConnectedPlayers.AddUnique(NewPlayer);

	if (LobbyGameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Array Num: %d"), LobbyGameState->PlayerArray.Num())
	}
}

void ABGLobbyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	UE_LOG(LogTemp, Warning, TEXT("Player Left"))

	for (int i{}; i < ConnectedPlayers.Num(); ++i)
	{
		if (ConnectedPlayers[i] == Exiting)
		{
			ConnectedPlayers.RemoveAt(i);
			GetGameState<ABGLobbyGameStateBase>()->RemovePlayerInfoFromArray(i);
			UE_LOG(LogTemp, Warning, TEXT("Removed Player from Connected Players Array"))
		}
	}
}
