// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BGGameStateBase.h"
#include "Core/BGTypes.h"

#include "BGLobbyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API ABGLobbyGameStateBase : public ABGGameStateBase
{
	GENERATED_BODY()

public:

	void AddPlayerInfoToArray(FBGPlayerInfo const& InPlayerInfo);

	int RemovePlayerInfoFromArray(FBGPlayerInfo const& InPlayerInfo);

	FBGServerData GetServerData() const { return ServerData; }
	TArray<FBGPlayerInfo> GetConnectedPlayersInfo() const { return ConnectedPlayersInfo; }

	void SetServerData(FBGServerData const& InServerData) { ServerData = InServerData; }

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_ConnectedPlayersInfo();

	UPROPERTY(ReplicatedUsing = OnRep_ConnectedPlayersInfo, EditAnywhere, BlueprintReadWrite, Category = "BGGameStateBase|Config")
	TArray<FBGPlayerInfo> ConnectedPlayersInfo;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly,
		Category = "BGGameStateBase|Config")
	FBGServerData ServerData;
};
