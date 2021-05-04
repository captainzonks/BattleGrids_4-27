// © 2021 Matthew Barham. All Rights Reserved.


#include "Core/Gameplay/BGGameplayGameStateBase.h"

#include "Actors/BGActor.h"
#include "Characters/BGCharacter.h"
#include "Components/BGSplineWallComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


TArray<ABGActor*> ABGGameplayGameStateBase::GetSpawnedActorsOfType(EBGActorType const& ActorType)
{
	TArray<ABGActor*> AllSpawnedActorsOfType;
	
	for (auto Actor : AllBGActors)
	{
		if (Actor->GetActorType() == ActorType)
		{
			AllSpawnedActorsOfType.Add(Actor);
		}
	}

	return AllSpawnedActorsOfType;
}

void ABGGameplayGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	// RefreshCurrentWorldState();
}

void ABGGameplayGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGGameplayGameStateBase, AllBGActors)
	DOREPLIFETIME(ABGGameplayGameStateBase, AllBGCharacters)
}

void ABGGameplayGameStateBase::RefreshCurrentWorldState()
{
	/** populate AllBGActors array with all ABGActor actors in the world */
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ABGActor::StaticClass(), Actors);

	for (auto Actor : Actors)
	{
		auto const BGActor = Cast<ABGActor>(Actor);
		if (BGActor)
		{
			AllBGActors.AddUnique(BGActor);
		}
	}

	/** populate AllBGCharacters array with all ABGCharacter characters in the world */
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(this, ABGCharacter::StaticClass(), Characters);

	for (auto Character : Characters)
	{
		auto const BGCharacter = Cast<ABGCharacter>(Character);
		if (BGCharacter)
		{
			AllBGCharacters.AddUnique(BGCharacter);
		}
	}
}

TArray<ABGCharacter*> ABGGameplayGameStateBase::GetSpawnedCharacters()
{
	return AllBGCharacters;
}

void ABGGameplayGameStateBase::AddSpawnedEntityToArray(ABGActor* ActorToAdd)
{
	if (ActorToAdd)
	{
		AllBGActors.AddUnique(ActorToAdd);
	}
}

void ABGGameplayGameStateBase::AddSpawnedEntityToArray(ABGCharacter* CharacterToAdd)
{
	if (CharacterToAdd)
	{
		AllBGCharacters.AddUnique(CharacterToAdd);
	}
}

void ABGGameplayGameStateBase::UpdateSplineData_Implementation(ABGActor* InActor)
{
	if (InActor)
	{
		auto const Component = InActor->GetComponentByClass(UBGSplineWallComponent::StaticClass());
		if (Component)
		{
			auto SplineWallComponent = Cast<UBGSplineWallComponent>(Component);
			if (SplineWallComponent)
			{
				SplineWallComponent->UpdateSplineCurveVariable();
			}
		}
	}
}