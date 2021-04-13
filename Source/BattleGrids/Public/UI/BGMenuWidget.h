// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "UI/BGMenuInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BGMenuWidget.generated.h"

class UBGThinkingPopup;

/**
 * 
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
