// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BGSaveGame.h"
#include "Core/BGTypes.h"

#include "BGGameSave.generated.h"

/**
 * BattleGrids Game Save
 */
UCLASS()
class BATTLEGRIDS_API UBGGameSave : public UBGSaveGame
{
	GENERATED_BODY()

public:

	void SaveCharacterModelInfo(FBGCharacterModelSaveInfo const InCharacterInfo);
	void SaveWallSplineInfo(FBGWallSplineSaveInfo const InWallSplineInfo);

	TArray<FBGCharacterModelSaveInfo> GetSavedCharacterModelInfoArray() const { return SavedCharacterModelInfo; }
	TArray<FBGWallSplineSaveInfo> GetSavedWallSplineInfoArray() const { return SavedWallSplineInfo; }

protected:

	UPROPERTY()
	TArray<FBGCharacterModelSaveInfo> SavedCharacterModelInfo;

	UPROPERTY()
	TArray<FBGWallSplineSaveInfo> SavedWallSplineInfo;
};