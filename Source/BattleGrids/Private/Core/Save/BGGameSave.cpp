// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/Save/BGGameSave.h"

void UBGGameSave::SaveCharacterModelInfo(FBGCharacterModelSaveInfo const InCharacterInfo)
{
	SavedCharacterModelInfo.AddUnique(InCharacterInfo);
}

void UBGGameSave::SaveWallSplineInfo(FBGWallSplineSaveInfo const InWallSplineInfo)
{
	SavedWallSplineInfo.AddUnique(InWallSplineInfo);
}
