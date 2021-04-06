// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/Lobby/BGLobbyGameStateBase.h"
#include "UI/BGLobbyMenu.h"
#include "Core/BGGameInstance.h"

#include "Engine/DemoNetDriver.h"

void ABGLobbyGameStateBase::AddPlayerInfoToArray(FBGPlayerInfo const& InPlayerInfo)
{
	ConnectedPlayersInfo.AddUnique(InPlayerInfo);

	if (HasAuthority())
	{
		GetGameInstance<UBGGameInstance>()->RefreshConnectedPlayersList(ConnectedPlayersInfo);
	}
}

int ABGLobbyGameStateBase::RemovePlayerInfoFromArray(FBGPlayerInfo const& InPlayerInfo)
{
	auto const Var = ConnectedPlayersInfo.RemoveSingle(InPlayerInfo);

	if (HasAuthority())
	{
		GetGameInstance<UBGGameInstance>()->RefreshConnectedPlayersList(ConnectedPlayersInfo);
	}
	
	return Var;
}

void ABGLobbyGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	auto const GameInstance = Cast<UBGGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		ServerData = GameInstance->GetInitialServerData();
	}
}

void ABGLobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGLobbyGameStateBase, ConnectedPlayersInfo)
	DOREPLIFETIME(ABGLobbyGameStateBase, ServerData)
}

void ABGLobbyGameStateBase::OnRep_ConnectedPlayersInfo()
{
	GetGameInstance<UBGGameInstance>()->RefreshConnectedPlayersList(ConnectedPlayersInfo);
}
