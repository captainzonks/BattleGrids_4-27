// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGGameModeBase.h"
#include "Core/BGTypes.h"
#include "BGGameplayGameModeBase.generated.h"

class ABGBoard;
class ABGPlayerState;
class ABGTile;
class ABGToken;
class ABGSplineStructure;
class ABGStructure;
class UDataTable;

/**
 * Gameplay GameModeBase class used for game levels in BattleGrids
 */
UCLASS()
class BATTLEGRIDS_API ABGGameplayGameModeBase : public ABGGameModeBase
{
	GENERATED_BODY()

public:

	TArray<FName> GetRowNamesOfType(EBGObjectType const& ObjectType) const;


	/// Tokens

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	void SpawnTokenAtLocation(FVector const& Location, FName const& MeshName, FName const& MaterialName);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void SetTokenPhysicsAndCollision(ABGToken* TokenToModify, bool const bPhysicsOn, bool const bGravityOn,
	                                        ECollisionEnabled::Type const CollisionType);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void MoveTokenToLocation(ABGToken* TokenToMove, FVector const& Location, FRotator const& ActorRotation);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void ToggleTokenLockInPlace(ABGToken* TokenToToggle, bool bLock);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void ToggleTokenPermissionsForPlayer(ABGPlayerState* PlayerStateToToggle, ABGToken* TokenToToggle);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void DestroyToken(ABGToken* TokenToDestroy);

	/// Structures

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	void SpawnSplineStructureAtLocation(FVector const& Location,
	                              FName const& WallStaticMeshName, FName const& WallMaskedMaterialInstanceName,
	                              FName const& CornerStaticMeshName, FName const& CornerMaskedMaterialInstanceName,
	                              FName const& BaseStaticMeshName, FName const& BaseMaterialInstanceName);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void SetSplineStructurePhysicsAndCollision(ABGSplineStructure* StructureToModify, bool const bGravityOn,
	                                            ECollisionEnabled::Type const CollisionType);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ModifySplineStructureLength(ABGSplineStructure* StructureToModify, int const& PointIndex,
	                                  FVector const& NewLocation);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void AddSplinePointToSplineStructure(ABGSplineStructure* StructureToModify, FVector const& ClickLocation,
	                                            int const& Index);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void AddPointToSpline(ABGSplineStructure* StructureToModify, FVector const& ClickLocation,
	                             float const& Key);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void MoveSplineStructure(ABGSplineStructure* StructureToMove, FVector const& Location);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void RemoveSplineStructureInstanceAtIndex(ABGSplineStructure* StructureToModify, FString const& InstanceName,
	                                           int const& Index);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ModifySplineStructureInstanceMeshAtIndex(ABGSplineStructure* StructureToModify, int const& Index,
	                                      FString const& NewInstanceName,
	                                      UStaticMesh* StaticMesh,
	                                      UMaterialInstance* MaterialInstance,
	                                      FString const& OldInstanceName = "WallInstance");

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void SpawnStructureActorAtSplineStructureIndex(ABGSplineStructure* StructureToModify, int const& Index,
	                                                      TSubclassOf<ABGStructure> StructureClassToSpawn,
	                                                      FString const& OldInstanceName = "WallInstance");

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void RestoreSplineStructureAtIndex(ABGStructure* StructureToRemove);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ToggleLockSplineStructureInPlace(ABGSplineStructure* StructureToLock, bool const bLock);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ResetSplineStructure(ABGSplineStructure* StructureToReset);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void DestroySplineStructure(ABGSplineStructure* StructureToDestroy);

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
	UDataTable* StaticMeshBank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	UDataTable* MaterialInstanceBank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	TArray<class ABGBoard*> AllBoards;
};
