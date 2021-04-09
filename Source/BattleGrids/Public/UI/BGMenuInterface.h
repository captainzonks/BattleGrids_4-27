// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BGMenuInterface.generated.h"

struct FBGServerData;
struct FBGPlayerInfo;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBGMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLEGRIDS_API IBGMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Host(FString const& ServerName) = 0;
	virtual void Join(uint32 const& Index, FBGServerData const& InServerData) = 0;
	virtual void LoadMainMenu() = 0;
	virtual void RefreshServerList() = 0;
	virtual void RefreshPlayerLists(TArray<FBGPlayerInfo> const& InPlayerInfo) = 0;
};
