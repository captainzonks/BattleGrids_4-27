// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/Lobby/BGLobbyGameStateBase.h"
#include "UI/BGLobbyMenu.h"
#include "Core/BGGameInstance.h"
#include "Core/Lobby/BGLobbyPlayerController.h"

#include "Engine/DemoNetDriver.h"

void ABGLobbyGameStateBase::AddPlayerInfoToArray(FBGPlayerInfo const& InPlayerInfo)
{
	ConnectedPlayersInfo.Add(InPlayerInfo);
}

void ABGLobbyGameStateBase::RemovePlayerInfoFromArray(int const& Index)
{
	UE_LOG(LogTemp, Warning, TEXT("ConnectedPlayersInfo.Num() : %s"), *FString::FromInt(ConnectedPlayersInfo.Num()))
	ConnectedPlayersInfo.RemoveAt(Index);
	UE_LOG(LogTemp, Warning, TEXT("ConnectedPlayersInfo.Num() : %s"), *FString::FromInt(ConnectedPlayersInfo.Num()))
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

void ABGLobbyGameStateBase::OnRep_ConnectedPlayersInfo() const
{
	if (auto LobbyPC = Cast<ABGLobbyPlayerController>(GetGameInstance()->GetFirstLocalPlayerController()))
	{
		LobbyPC->SetLobbyNeedsUpdating(true);
	}
}

//
// void ABGLobbyGameStateBase::OnRep_ConnectedPlayersInfo()
// {
// 	GetGameInstance<UBGGameInstance>()->RefreshConnectedPlayersList(ConnectedPlayersInfo);
// }
