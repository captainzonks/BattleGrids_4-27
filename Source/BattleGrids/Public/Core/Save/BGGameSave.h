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

	UFUNCTION(BlueprintCallable, Category = "BGGameSave|Functions")
	void SaveCharacterModelInfo(FBGCharacterModelSaveInfo const InCharacterInfo);

	TArray<FBGCharacterModelSaveInfo> GetSavedCharacterModelInfoArray() const { return SavedCharacterModelInfo; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameSave|Config")
	TArray<FBGCharacterModelSaveInfo> SavedCharacterModelInfo;
	
};
