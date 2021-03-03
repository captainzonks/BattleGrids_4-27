// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "BGSplineStructure.generated.h"

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
	void SetLocationOfSplinePoint(int const& PointIndex, FVector const& NewLocation);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddPointToSplineAtIndex(FVector const& Location, int const& Index);

	///////////////////////
	/// Getters

	class USplineComponent* GetSplineComponent() const { return SplineComponent; }

	class UInstancedStaticMeshComponent* GetInstancedStaticMeshComponent() const { return InstancedStaticMeshComponent; }

	class UStaticMesh* GetStaticMeshReference() const { return StaticMeshReference; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	class UStaticMesh* StaticMeshReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	class UMaterialInstance* MaterialReference;
};
