// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGGameModeBase.h"
#include "Core/BGTypes.h"

#include "BGLobbyGameModeBase.generated.h"

class UBGGameInstance;

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API ABGLobbyGameModeBase : public ABGGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void UpdateAllConnectedPlayers();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	TArray<FBGPlayerInfo> ConnectedPlayersInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	APlayerController* NewlyConnectedPlayer;

	UPROPERTY()
	UBGGameInstance* GameInstance;

	UPROPERTY()
	FBGServerData ServerData;

	bool ServerReady {false};
};
