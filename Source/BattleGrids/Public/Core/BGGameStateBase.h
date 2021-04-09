// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGTypes.h"
#include "GameFramework/GameStateBase.h"
#include "BGGameStateBase.generated.h"

class ABGBoard;

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API ABGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	// ConnectedPlayersInfo

	TArray<FBGPlayerInfo> GetConnectedPlayersInfo() const { return ConnectedPlayersInfo; }
	void AddPlayerInfoToArray(FBGPlayerInfo const& InPlayerInfo);
	void RemovePlayerInfoFromArray(int const& Index);

	// ServerData

	FBGServerData GetServerData() const { return ServerData; }
	void SetServerData(FBGServerData const& InServerData) { ServerData = InServerData; }



protected:

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGGameStateBase|Config")
	TArray<FBGPlayerInfo> ConnectedPlayersInfo;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGGameStateBase|Config")
	FBGServerData ServerData;
};