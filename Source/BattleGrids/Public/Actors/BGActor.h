// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Core/BGActorInterface.h"
#include "GameFramework/Actor.h"
#include "BGActor.generated.h"

class UWidgetComponent;

/**
 * BattleGrands custom Actor from which all in-game objects inherit
 */
UCLASS()
class BATTLEGRIDS_API ABGActor : public AActor, public IBGActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABGActor();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetWidgetComponentClass(TSubclassOf<UUserWidget> InClass);

	//////////////////////
	/// Interface Implementation

	virtual void ToggleContextMenu() override;
	virtual void CloseContextMenu() override;
	virtual UBGContextMenu* GetContextMenu() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void UpdateContextMenuWidget();

	///////////////////
	/// Replication Functions

	UFUNCTION()
	void OnRep_ContextMenuClass();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ContextMenuWidgetComponent;

	UPROPERTY(ReplicatedUsing = OnRep_ContextMenuClass, EditAnywhere, BlueprintReadWrite,
		meta = (ExposeOnSpawn = "true"), Category = "BGActor|Config")
	TSubclassOf<UUserWidget> ContextMenuClass;
};
