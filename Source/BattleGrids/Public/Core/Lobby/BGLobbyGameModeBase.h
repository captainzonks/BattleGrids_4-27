// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BGGameModeBase.h"
#include "Core/BGTypes.h"

#include "BGLobbyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API ABGLobbyGameModeBase : public ABGGameModeBase
{
	GENERATED_BODY()
public:

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	FString ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	int CurrentNumberOfPlayers {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	int MaxNumberOfPlayers {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	TArray<FBGPlayerInfo> ConnectedPlayersInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	APlayerController* NewlyConnectedPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGLobbyGameModeBase|Config")
	TArray<APlayerController*> AllConnectedPlayers;
};
