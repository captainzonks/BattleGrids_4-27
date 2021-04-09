// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/Gameplay/BGGamePlayerState.h"

void ABGGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		ClientSetPlayerInfo(GetPlayerInfo());
	}
}
