// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "Core/BGTypes.h"
#include "GameFramework/Actor.h"
#include "BGSplineStructure.generated.h"

class UInstancedStaticMeshComponent;
class USceneComponent;
class USplineComponent;
class UStaticMesh;

UCLASS()
class BATTLEGRIDS_API ABGSplineStructure : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABGSplineStructure();

	////////////////////////
	/// Public Functions

	// Multicast, toggles physics and collision on the structure
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void SetStructurePhysicsAndCollision(bool const bGravityOn,
	                                     ECollisionEnabled::Type const CollisionType);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void UpdateStructureMesh();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void ModifyInstanceMeshAtIndex(int const& Index, FString const& NewInstanceName, UStaticMesh* StaticMesh,
	                               UMaterialInstance* MaterialInstance,
	                               FString const& OldInstanceName = "WallInstance");

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void SetLocationOfSplinePoint(int const& PointIndex, FVector const& NewLocation);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddPointToSpline(FVector const& Location, int const& Key);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddPointToSplineAtIndex(FVector const& Location, int const& Index);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void ToggleLockStructureInPlace(bool const bLock);

	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddStructureInfoToArray(UStaticMesh* StaticMesh, UMaterialInstance* MaterialInstance,
	                             FTransform const& Transform);

	///////////////////////
	/// Getters

	USplineComponent* GetSplineComponent() const { return SplineComponent; }

	// Searches the Actor's Instances array for an Instance component with the ObjectName matching the provided string.
	// Returns a nullptr if no match is found.
	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponentByString(FString const& InstanceName) const;

	TArray<FBGStructureInfo> GetStructureDataArray() const { return StructureDataArray; }

	uint8 GetLockedStatus() const { return bLocked; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void CleanupInstanceComponents() const;

	// Creates an InstancedStaticMeshComponent with the provided Name, Mesh, and Material
	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	UInstancedStaticMeshComponent* CreateInstancedStaticMeshComponent(FString const& InstanceName,
	                                                                  UStaticMesh* StaticMesh,
	                                                                  UMaterialInstance* MaterialInstance);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

	// stores the main structural meshes and material information
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	TArray<FBGStructureInfo> StructureDataArray;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	uint8 bLocked : 1;
};
