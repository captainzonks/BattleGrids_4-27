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

	UFUNCTION(BlueprintCallable, Category = "BGSplineStructure|Functions")
	void AddMeshToSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USplineComponent* SplineComponent;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	// class UInstancedStaticMeshComponent* InstancedStaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	class UStaticMesh* StaticMeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGSplineStructure|Config")
	class UMaterialInstance* MaterialReference;
};
