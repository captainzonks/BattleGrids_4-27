// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGStructure.h"


#include "Actors/BGSplineStructure.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABGStructure::ABGStructure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	DoorFrameStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Frame Static Mesh"));
	RootComponent = DoorFrameStaticMeshComponent;

	DoorStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Static Mesh"));
	DoorStaticMeshComponent->SetupAttachment(DoorFrameStaticMeshComponent);
	DoorStaticMeshComponent->SetRelativeLocation(FVector(40.f, 0.f, 10.f));

	bLocked = false;
}

void ABGStructure::SetParentSplineStructure(ABGSplineStructure* NewParentSplineStructure)
{
	ParentSplineStructure = NewParentSplineStructure;
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

// Called every frame
void ABGStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABGStructure::ToggleStructureLock_Implementation(bool const bNewLocked)
{
	SetLocked(bNewLocked);
}
