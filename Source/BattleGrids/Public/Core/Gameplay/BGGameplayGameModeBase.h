// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGGameModeBase.h"
#include "Core/BGTypes.h"
#include "BGGameplayGameModeBase.generated.h"

class ABGBoard;
class ABGPlayerState;
class ABGTile;

/**
 * Gameplay GameModeBase class used for game levels in BattleGrids
 */
UCLASS()
class BATTLEGRIDS_API ABGGameplayGameModeBase : public ABGGameModeBase
{
	GENERATED_BODY()

public:
	/// Tokens

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	void SpawnTokenAtLocation(FVector const& Location, FName const& RowName);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void SetTokenPhysicsAndCollision(ABGToken* TokenToMove, bool const bPhysicsOn, bool const bGravityOn,
	                                        ECollisionEnabled::Type const CollisionType);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void MoveTokenToLocation(ABGToken* TokenToMove, AActor* TargetActor, FHitResult const& TargetHitResult,
	                                bool const& bHolding,
	                                FRotator const& ActorRotation);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void ToggleTokenLockInPlace(ABGToken* TokenToToggle, bool bLock);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void ToggleTokenPermissionsForPlayer(ABGPlayerState* PlayerStateToToggle, ABGToken* TokenToToggle);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void DestroyToken(ABGToken* TokenToDestroy);

	/// Structures

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	void SpawnStructureAtLocation(FVector const& Location, FName const& RowName);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ModifyStructureLength(ABGSplineStructure* StructureToModify, int const& PointIndex,
	                                  FVector const& NewLocation);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void AddSplinePointToStructureSpline(ABGSplineStructure* StructureToModify, FVector const& ClickLocation,
	                                            int const& Index);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void RemoveStructureInstanceAtIndex(ABGSplineStructure* StructureToModify, int const& Index);

	/// Boards

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	void SpawnNewBoard(int const& Zed, int const& X, int const& Y);

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	static void ToggleTileVisibility(ABGTile* TileToToggle);

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	static void ShrinkBoard(ABGBoard* BoardToShrink);

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	static void GrowBoard(ABGBoard* BoardToGrow);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	class UDataTable* TokenDataBank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	class UDataTable* StructureDataBank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	TArray<class ABGBoard*> AllBoards;
};
