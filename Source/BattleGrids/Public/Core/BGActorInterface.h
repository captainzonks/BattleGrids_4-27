// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BGActorInterface.generated.h"

class UBGContextMenu;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBGActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLEGRIDS_API IBGActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void ToggleContextMenu() = 0;
	virtual void CloseContextMenu() = 0;
	virtual UBGContextMenu* GetContextMenu() = 0;
};
