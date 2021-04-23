// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Actors/BGActor.h"
#include "GameFramework/Character.h"
#include "BGCharacter.generated.h"

UCLASS()
class BATTLEGRIDS_API ABGCharacter : public ACharacter, public IBGActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABGCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Destroyed() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//////////////////////
	/// Setters

	UFUNCTION(Server, Reliable)
	void SetWidgetComponentClass(TSubclassOf<UUserWidget> InClass);

	//////////////////////
	/// Interface Implementation

	virtual void ToggleContextMenu() override;
	virtual void CloseContextMenu() override;
	virtual UBGContextMenu* GetContextMenu() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
	void SetFacadeActorContextMenuParent();

	UFUNCTION()
	void OnRep_FacadeActor();

	UPROPERTY(ReplicatedUsing=OnRep_FacadeActor, VisibleAnywhere, BlueprintReadOnly, Category = "BGCharacter|Config")
	ABGActor* FacadeActor;
};