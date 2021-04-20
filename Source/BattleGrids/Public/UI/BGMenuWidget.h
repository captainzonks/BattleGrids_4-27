// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGMenuWidget.generated.h"

class IBGMenuInterface;

/**
 * Parent widget class for all Menus in BattleGrids
 */
UCLASS()
class BATTLEGRIDS_API UBGMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Setup();
	void Teardown();

	void SetMenuInterface(IBGMenuInterface* NewMenuInterface);

protected:
	IBGMenuInterface* MenuInterface;
};
