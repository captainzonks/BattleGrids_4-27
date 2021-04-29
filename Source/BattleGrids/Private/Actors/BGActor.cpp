// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGActor.h"

#include "Components/BGSplineWallComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/BGContextMenu.h"

// Sets default values
ABGActor::ABGActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	RootComponent = RootSceneComponent;
}

// Called when the game starts or when spawned
void ABGActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicatingMovement(true);
}

void ABGActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGActor, ActorType)
}

void ABGActor::SetActorType(EBGActorType const& InActorType)
{
	ActorType = InActorType;
}