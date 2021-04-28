// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Core/BGTypes.h"

#include "BGSplineWallComponent.generated.h"

class ABGStructure;
class UBoxComponent;
class USplineComponent;

UCLASS( ClassGroup=(BattleGrids), meta=(BlueprintSpawnableComponent) )
class BATTLEGRIDS_API UBGSplineWallComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBGSplineWallComponent();

	////////////////////////
	/// Public Functions

	/**
	 * Primary function which cleans up all Instances and rebuilds all InstancedStaticMeshComponents for all Splines
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void UpdateSplineStructureMesh(int const& PointIndex = 0);

	/**
	 * Removes Instance mesh at Index from specified InstancedStaticMeshComponent name
	 * This adds a new InstancedMeshComponent, or Instance to an existing Component, 
	 * of supplied StaticMesh and MaterialInstance
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void ModifyInstanceMeshAtIndex(int const& Index, FString const& NewInstanceName, UStaticMesh* StaticMesh,
	                               UMaterialInstance* MaterialInstance,
	                               FString const& OldInstanceName = "WallInstance");

	/**
	 * Spawns a BGStructure Actor at the provided Index
	 * This removes an Instance from the InstancedStaticMeshComponent
	 */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void SpawnStructureActorAtIndex(int const& Index, TSubclassOf<ABGStructure> StructureClassToSpawn,
	                                FTransform const& SpawnTransform,
	                                FString const& OldInstanceName);

	/**
	 * Restores an Instance Mesh
	 * By default it is the WallInstance that is restored
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void RestoreInstanceMeshAtIndex(int const& Index, FTransform const& Transform,
	                                FString const& InstanceName = "WallInstance");

	/**
	 * Removes the Instance at the provided Index
	 * By default it removes from the WallInstance
	 */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void RemoveInstanceMeshAtIndex(int const& Index, FString const& InstanceName = "WallInstance");

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void SetLocationOfSplinePoint(int const& PointIndex, FVector const& NewLocation);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddPointToSpline(FVector const& Location, float const& Key);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddPointToSplineAtIndex(FVector const& Location, int const& Index);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void ToggleLockStructureInPlace(bool const bNewLocked);

	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	void RemoveStructureFromArray(ABGStructure* StructureToRemove);

	///////////////////////
	/// Getters

	USplineComponent* GetSplineComponent() const { return SplineComponent; }

	/**
	* Searches the Actor's Instances array for an Instance component with the ObjectName matching the provided string.
	* Returns a nullptr if no match is found.
	*/
	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponentByTag(FString const& Tag) const;

	FBGStructureInfo GetStructureData() const { return StructureData; }

	uint8 GetLocked() const { return bLocked; }

	///////////////////////
	/// Setters

	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	void SetStructureInfo(FBGStructureInfo const& NewStructureInfo);

	void SetLocked(bool const bNewLocked) { bLocked = bNewLocked; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void CleanupInstanceComponents(int const& PointIndex = 0);

	void UpdateWallMesh();

	void UpdateCornerMesh();

	/** Creates an InstancedStaticMeshComponent with the provided Name, Mesh, and Material */
	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	UInstancedStaticMeshComponent* CreateInstancedStaticMeshComponent(FString const& InstanceTag,
																	UStaticMesh* StaticMesh,
																	UMaterialInstance* MaterialInstance);

	//////////////////////
	/// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

	////////////////////
	/// Member Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BGSplineWallComponent|Config")
	TArray<UBoxComponent*> CollisionBoxes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BGSplineWallComponent|Config")
	TArray<UBoxComponent*> DisabledCollisionBoxes;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "BGSplineWallComponent|Config")
	TArray<ABGStructure*> SpawnedStructures;

	/** Stores the main structural meshes and material information */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGSplineWallComponent|Config")
	FBGStructureInfo StructureData;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "BGSplineWallComponent|Config")
	uint8 bLocked : 1;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "BGSplineWallComponent|Config")
	int UniqueCollisionNumber{};

	UPROPERTY(Replicated, VisibleAnywhere, Category = "BGSplineWallComponent|Config")
	int UniqueInstanceNumber{};
};
