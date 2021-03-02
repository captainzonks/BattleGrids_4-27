// © 2021 Matthew Barham. All Rights Reserved.

#include "Core/Gameplay/BGGameplayGameModeBase.h"

#include "Actors/BGSplineStructure.h"
#include "Actors/BGToken.h"


void ABGGameplayGameModeBase::SetTokenPhysicsAndCollision(ABGToken* TokenToMove, bool const bPhysicsOn,
                                                          bool const bGravityOn,
                                                          ECollisionEnabled::Type const CollisionType)
{
	TokenToMove->ToggleTokenPhysicsAndCollision(bPhysicsOn, bGravityOn, CollisionType);
}

void ABGGameplayGameModeBase::MoveTokenToLocation(ABGToken* TokenToMove, AActor* TargetActor, bool const& bHolding,
                                                  FRotator const& ActorRotation)
{
	float ZedValue;
	bHolding ? ZedValue = 100.f : ZedValue = 50.f;

	FVector ActorOrigin{};
	FVector ActorBoxExtent{};
	TargetActor->GetActorBounds(false, ActorOrigin, ActorBoxExtent, false);

	FVector Location;
	Location.X = ActorOrigin.X;
	Location.Y = ActorOrigin.Y;
	Location.Z = ZedValue + ActorOrigin.Z + ActorBoxExtent.Z;

	TokenToMove->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
	TokenToMove->SetActorRotation(ActorRotation, ETeleportType::TeleportPhysics);
}

void ABGGameplayGameModeBase::ModifyStructureLength(ABGSplineStructure* StructureToModify,
                                                    int const& PointIndex,
                                                    FVector const& NewLocation)
{
	StructureToModify->SetLocationOfSplinePoint(PointIndex, NewLocation);
	StructureToModify->AddMeshToSpline();
}

void ABGGameplayGameModeBase::AddSplinePointToStructureSpline(ABGSplineStructure* StructureToModify,
                                                              FVector const& ClickLocation, int const& Index)
{
	StructureToModify->AddPointToSplineAtIndex(ClickLocation, Index);
}
