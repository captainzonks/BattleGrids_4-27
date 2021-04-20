// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/BGMenuWidget.h"
#include "BGContextMenu.generated.h"

/**
 * Parent class for Context Menus, inherits from BGMenuWidget
 */
UCLASS()
class BATTLEGRIDS_API UBGContextMenu : public UBGMenuWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "BGContextMenu|Events")
	void Update();

	FHitResult GetHitResult() const { return HitResult; }
	
	void SetParent(AActor* InParent) { Parent = InParent; }
	void SetHitResult(FHitResult const& InHitResult) { HitResult = InHitResult; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "BGContextMenu|Config")
	AActor* Parent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "BGContextMenu|Config")
	FHitResult HitResult;
};