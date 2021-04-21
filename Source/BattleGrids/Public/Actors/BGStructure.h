// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGActorInterface.h"
#include "GameFramework/Actor.h"
#include "BGStructure.generated.h"

class UWidgetComponent;
class ABGSplineStructure;
class UStaticMeshComponent;

UCLASS()
class BATTLEGRIDS_API ABGStructure : public AActor, public IBGActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABGStructure();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BGStructure|Network")
	void ToggleStructureLock(bool const bNewLocked);

	//////////////////////
	/// Interface Implementation

	virtual void ToggleContextMenu() override;
	virtual void CloseContextMenu() override;
	virtual UBGContextMenu* GetContextMenu() override;

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

	UFUNCTION(Server, Reliable)
	void SetWidgetComponentClass(TSubclassOf<UUserWidget> InClass);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateContextMenuWidget();

	/////////////////
	/// Replication Functions

	UFUNCTION()
    void OnRep_ContextMenuClass();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ContextMenuWidgetComponent;

	UPROPERTY(ReplicatedUsing=OnRep_ContextMenuClass, EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"),
		Category = "BGStructure|Config")
	TSubclassOf<UUserWidget> ContextMenuClass;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGStructure|Config")
	ABGSplineStructure* ParentSplineStructure{nullptr};

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGStructure|Config")
	int IndexOnSplineStructure{};

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "BGStructure|Config")
	uint8 bLocked : 1;
};
