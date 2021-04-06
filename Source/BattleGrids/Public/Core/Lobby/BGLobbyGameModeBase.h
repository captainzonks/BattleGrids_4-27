// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGGameModeBase.h"

#include "BGLobbyGameModeBase.generated.h"

class UBGGameInstance;
class ABGLobbyGameStateBase;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BGLobbyGameModeBase|Config")
	ABGLobbyGameStateBase* LobbyGameState;
};