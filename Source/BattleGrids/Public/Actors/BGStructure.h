// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BGStructure.generated.h"

class ABGSplineStructure;
class UStaticMeshComponent;

UCLASS()
class BATTLEGRIDS_API ABGStructure : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABGStructure();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/////////////////
	/// Getters

	ABGSplineStructure* GetParentSplineStructure() const { return ParentSplineStructure; }

	int GetIndexOnSplineStructure() const { return IndexOnSplineStructure; }

	/////////////////
	/// Setters

	void SetParentSplineStructure(ABGSplineStructure* NewParentSplineStructure)
	{
		ParentSplineStructure = NewParentSplineStructure;
	}

	void SetIndexOnSplineStructure(int const Index) { IndexOnSplineStructure = Index; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorFrameStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* DoorStaticMeshComponent;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "BGStructure|Config")
	ABGSplineStructure* ParentSplineStructure{nullptr};

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "BGStructure|Config")
	int IndexOnSplineStructure {};

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "BGStructure|Config")
	uint8 bLocked : 1;
};