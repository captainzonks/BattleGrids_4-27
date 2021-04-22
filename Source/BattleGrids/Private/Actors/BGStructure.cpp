// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGStructure.h"


#include "Actors/BGSplineStructure.h"
#include "Net/UnrealNetwork.h"
#include "UI/BGContextMenu.h"

// Sets default values
ABGStructure::ABGStructure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	bLocked = false;
}

// Called when the game starts or when spawned
void ABGStructure::BeginPlay()
{
	Super::BeginPlay();

	SetReplicatingMovement(true);
}

void ABGStructure::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGStructure, bLocked)
	DOREPLIFETIME(ABGStructure, ParentSplineStructure)
	DOREPLIFETIME(ABGStructure, IndexOnSplineStructure)
}

void ABGStructure::SetParentSplineStructure(ABGSplineStructure* NewParentSplineStructure)
{
	ParentSplineStructure = NewParentSplineStructure;
}

void ABGStructure::ToggleStructureLock_Implementation(bool const bNewLocked)
{
	SetLocked(bNewLocked);
}
