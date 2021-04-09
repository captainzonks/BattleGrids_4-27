// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BGGameStateBase.h"
#include "BGGameplayGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API ABGGameplayGameStateBase : public ABGGameStateBase
{
	GENERATED_BODY()
public:
	// Gameplay Data

	TArray<ABGBoard*> GetAllBoards() const { return AllBoards; }

protected:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGGameStateBase|Config",
		meta = (AllowPrivateAccess = true))
	TArray<ABGBoard*> AllBoards;
};
