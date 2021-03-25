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

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BGStructure|Network")
	void ToggleStructureLock(bool const bNewLocked);

	/////////////////
	/// Getters

	ABGSplineStructure* GetParentSplineStructure() const { return ParentSplineStructure; }

	uint8 GetLocked() const { return bLocked; }

	int GetIndexOnSplineStructure() const { return IndexOnSplineStructure; }

	/////////////////
	/// Setters

	void SetParentSplineStructure(ABGSplineStructure* NewParentSplineStructure);

	void SetLocked(bool const bNewLocked) { bLocked = bNewLocked; }

	void SetIndexOnSplineStructure(int const Index) { IndexOnSplineStructure = Index; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGStructure|Config")
	ABGSplineStructure* ParentSplineStructure{nullptr};

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGStructure|Config")
	int IndexOnSplineStructure {};

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "BGStructure|Config")
	uint8 bLocked : 1;
};