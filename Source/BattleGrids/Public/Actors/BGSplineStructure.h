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
	void AddMeshToSpline();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void ModifyInstanceMeshAtIndex(int const& Index, FString const& NewInstanceName, UStaticMesh* StaticMesh,
	                               UMaterialInstance* MaterialInstance,
	                               FString const& OldInstanceName = "WallInstance");

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void SetLocationOfSplinePoint(int const& PointIndex, FVector const& NewLocation);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddPointToSplineAtIndex(FVector const& Location, int const& Index);

	///////////////////////
	/// Getters

	USplineComponent* GetSplineComponent() const { return SplineComponent; }

	UInstancedStaticMeshComponent* GetInstancedStaticMeshComponentByString(FString const& InstanceName) const;

	// UInstancedStaticMeshComponent* GetWallInstancedStaticMeshComponent() const;

	TArray<FBGStructureInfo> GetPrimaryStructureDataArray() const { return PrimaryStructureDataArray; }
	// TArray<FBGStructureInfo> GetSecondaryStructureDataArray() const { return SecondaryStructureDataArray; }

	UStaticMesh* GetPrimaryStaticMeshReference() const { return PrimaryStructureDataArray[0].StaticMesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void CleanupInstanceComponents() const;

	UInstancedStaticMeshComponent* CreateInstancedStaticMeshComponent(FString const& InstanceName,
	                                                                  UStaticMesh* StaticMesh,
	                                                                  UMaterialInstance* MaterialInstance);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USplineComponent* SplineComponent;

	// stores the main structural meshes and material information
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	TArray<FBGStructureInfo> PrimaryStructureDataArray;

	// // stores extra meshes, such as Doors and Windows and other things that may need to be used
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	// TArray<FBGStructureInfo> SecondaryStructureDataArray;
};
