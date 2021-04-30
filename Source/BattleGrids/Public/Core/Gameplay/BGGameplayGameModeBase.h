// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGGameModeBase.h"
#include "Core/BGTypes.h"
#include "UI/BGContextMenu.h"

#include "BGGameplayGameModeBase.generated.h"

class UBGSplineWallComponent;
class ABGActor;
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
	/**
	 * ----NEEDS FURTHER WORK----
	 * Factory design pattern initial implementation
	 * Creates a BGActor of the specified type with components
	 */
	UFUNCTION(BlueprintCallable, Category = "BGGameplayGameModeBase|Gameplay")
	ABGActor* CreateNewActor(EBGActorType const& ActorType);

	ABGActor* CreateNewSplineWallActor();

	ABGActor* CreateNewTileActor();

	TArray<FName> GetRowNamesOfType(EBGClassCategory const& ClassCategory) const;

	UFUNCTION(BlueprintCallable, Category = "BGGameplayGameModeBase|Gameplay")
	void DestroyGameActor(AActor* ActorToDestroy);


	/**
	 * Token Functions
	 */

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	void SpawnTokenAtLocation(FVector const& Location, FName const& MeshName, FName const& MaterialName);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void MoveTokenToLocation(ABGToken* TokenToMove, FVector const& Location, FRotator const& ActorRotation);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void ToggleTokenLockInPlace(ABGToken* TokenToToggle, bool bLock);

	UFUNCTION(Category = "BGGameplayGameModeBase|Tokens")
	static void ToggleTokenPermissionsForPlayer(ABGPlayerState* PlayerStateToToggle, ABGToken* TokenToToggle);

	/**
	 * Spline Structure Functions
	 */

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	void CreateStructureInfo(FBGStructureInfo& OutStructureInfo,
	                         FName const& WallStaticMeshName, FName const& WallMaskedMaterialInstanceName,
	                         FName const& CornerStaticMeshName,
	                         FName const& CornerMaskedMaterialInstanceName,
	                         FName const& BaseStaticMeshName, FName const& BaseMaterialInstanceName,
	                         FVector const& Location);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ModifySplineStructureLength(UBGSplineWallComponent* InSplineComponent, int const& PointIndex,
	                                        FVector const& NewLocation);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void AddSplinePointToSplineStructure(UBGSplineWallComponent* InSplineComponent, FVector const& ClickLocation,
	                                            int const& Index);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void AddPointToSpline(UBGSplineWallComponent* InSplineComponent, FVector const& ClickLocation,
	                             float const& Key);

	// UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	// static void MoveSplineStructure(ABGSplineStructure* StructureToMove, FVector const& Location);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ModifySplineStructureInstanceMeshAtIndex(UBGSplineWallComponent* InSplineComponent, int const& Index,
	                                                     FString const& NewInstanceName,
	                                                     UStaticMesh* StaticMesh,
	                                                     UMaterialInstance* MaterialInstance,
	                                                     FString const& OldInstanceName = "WallInstance");

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void RestoreInstanceMeshOnSplineStructure(UBGSplineWallComponent* InSplineComponent,
	                                                 int const& Index,
	                                                 FTransform const& NewInstanceTransform,
	                                                 FString const& InstanceName = "WallInstance");

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void ResetSplineStructure(UBGSplineWallComponent* InSplineComponent);

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	void UpdateConnectedClientSplineData();

	/**
	 * Structure Functions
	 */

	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void SpawnStructureActorAtSplineStructureIndex(UBGSplineWallComponent* InSplineComponent, int const& Index,
	                                                      TSubclassOf<ABGStructure> StructureClassToSpawn,
	                                                      FString const& OldInstanceName = "WallInstance");

	// TODO: how can we make this less special? Do we need a specific destroy function for Doors, etc.?
	UFUNCTION(Category = "BGGameplayGameModeBase|Structures")
	static void DestroyStructureActor(ABGStructure* StructureToRemove);

	/**
	 * Board & Tile Functions
	 */

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	void SpawnNewBoard(int const& Zed, int const& X, int const& Y) const;

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	static void ToggleTileVisibility(ABGTile* TileToToggle);

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	static void ShrinkBoard(ABGBoard* BoardToShrink);

	UFUNCTION(Category = "BGGameplayGameModeBase|Boards")
	static void GrowBoard(ABGBoard* BoardToGrow);

	/**
	 * BGActor Functions
	 */

	UFUNCTION(Category = "BGGameplayGameModeBase|Actors")
	void SpawnNewActor(TSubclassOf<ABGActor> const ActorToSpawn) const;

	UFUNCTION(Category = "BGGameplayGameModeBase|Actors")
	static void MoveActorToLocation(AActor* ActorToMove, FTransform const& NewTransform);

	TSubclassOf<UBGContextMenu> GetStructureContextMenuClass() const { return StructureContextMenuClass; }
	TSubclassOf<UBGContextMenu> GetTileContextMenuClass() const { return TileContextMenuClass; }

protected:
	ABGGameplayGameModeBase();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	UDataTable* StaticMeshBank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	UDataTable* MaterialInstanceBank;

	/////////////////////
	/// Actor Array

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config")
	TArray<ABGActor*> SpawnedActors;

	/////////////////////
	/// Context Menu Class References

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config|UI")
	TSubclassOf<UBGContextMenu> TokenContextMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config|UI")
	TSubclassOf<UBGContextMenu> SplineStructureContextMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config|UI")
	TSubclassOf<UBGContextMenu> StructureContextMenuClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGameplayGameModeBase|Config|UI")
	TSubclassOf<UBGContextMenu> TileContextMenuClass;
};
