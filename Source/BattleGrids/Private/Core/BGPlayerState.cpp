// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/BGPlayerState.h"

#include "Engine/DemoNetDriver.h"

void ABGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGPlayerState, PlayerInfo)
	DOREPLIFETIME(ABGPlayerState, ServerData)
}

void ABGPlayerState::SetPlayerInfo(FBGPlayerInfo const& InPlayerInfo)
{
	PlayerInfo = InPlayerInfo;
}

void ABGPlayerState::SetServerData(FBGServerData const& InServerData)
{
	ServerData = InServerData;
}
