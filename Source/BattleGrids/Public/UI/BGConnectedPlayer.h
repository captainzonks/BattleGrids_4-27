// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/BGTypes.h"

#include "BGConnectedPlayer.generated.h"

class UBGLobbyMenu;
class UTextBlock;
class UButton;
class UImage;

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API UBGConnectedPlayer : public UUserWidget
{
	GENERATED_BODY()

public:


	void Setup(UBGLobbyMenu* InParent, uint32 const InIndex);

	FBGPlayerInfo GetPlayerInfo() const { return PlayerInfo; }

	void SetPlayerInfo(FBGPlayerInfo const& InPlayerInfo) { PlayerInfo = InPlayerInfo; }

	void UpdateButtonColor(bool const bReady) const;

protected:
	virtual bool Initialize() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UButton* ConnectedPlayerButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PlayerName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	UImage* PlayerColor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBGLobbyMenu* Parent;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected{false};

	UPROPERTY(BlueprintReadOnly)
	FBGPlayerInfo PlayerInfo;

	uint32 Index;
};
