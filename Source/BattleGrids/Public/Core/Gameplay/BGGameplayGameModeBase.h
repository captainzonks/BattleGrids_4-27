// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGGameModeBase.h"
#include "Core/BGTypes.h"
#include "BGGameplayGameModeBase.generated.h"

/**
 * Gameplay GameModeBase class used for game levels in BattleGrids
 */
UCLASS()
class BATTLEGRIDS_API ABGGameplayGameModeBase : public ABGGameModeBase
{
	GENERATED_BODY()

public:
	/// Tokens

	UFUNCTION(Category = "BGGameplayGameModeBase|Network")
	static void SetTokenPhysicsAndCollision(ABGToken* TokenToMove, bool const bPhysicsOn, bool const bGravityOn,
	                                        ECollisionEnabled::Type const CollisionType);

	UFUNCTION(Category = "BGGameplayGameModeBase|Network")
	static void MoveTokenToLocation(ABGToken* TokenToMove, AActor* TargetActor, bool const& bHolding,
	                                FRotator const& ActorRotation);

	/// Structures

	UFUNCTION(Category = "BGGameplayGameModeBase|Network")
	static void ModifyStructureLength(ABGSplineStructure* StructureToModify, int const& PointIndex,
	                                  FVector const& NewLocation);

	UFUNCTION(Category = "BGGameplayGameModeBase|Network")
	static void AddSplinePointToStructureSpline(ABGSplineStructure* StructureToModify, FVector const& ClickLocation,
	                                            int const& Index);
};
