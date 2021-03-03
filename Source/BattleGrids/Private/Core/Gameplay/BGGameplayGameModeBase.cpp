// © 2021 Matthew Barham. All Rights Reserved.

#include "Core/Gameplay/BGGameplayGameModeBase.h"


#include "Actors/BGBoard.h"
#include "Actors/BGSplineStructure.h"
#include "Actors/BGTile.h"
#include "Actors/BGToken.h"
#include "Components/SplineComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"


void ABGGameplayGameModeBase::SpawnTokenAtLocation(FVector const& Location, FName const& RowName)
{
	if (TokenDataBank)
	{
		auto const Row = TokenDataBank->FindRow<FBGTokenBank>(RowName, "");

		if (Row)
		{
			FTransform NewTransform{};
			NewTransform.SetLocation(Location);

			auto const TokenToSpawn = Cast<ABGToken>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
				this, Row->TokenClassReference, NewTransform,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
				this));

			TokenToSpawn->GetStaticMeshComponent()->SetSimulatePhysics(true);

			UGameplayStatics::FinishSpawningActor(TokenToSpawn, NewTransform);
		}
	}
}

void ABGGameplayGameModeBase::SetTokenPhysicsAndCollision(ABGToken* TokenToModify, bool const bPhysicsOn,
                                                          bool const bGravityOn,
                                                          ECollisionEnabled::Type const CollisionType)
{
	if (TokenToModify)
	{
		TokenToModify->SetTokenPhysicsAndCollision(bPhysicsOn, bGravityOn, CollisionType);
	}
}

void ABGGameplayGameModeBase::MoveTokenToLocation(ABGToken* TokenToMove, FVector const& Location,
                                                  FRotator const& ActorRotation)
{
	if (TokenToMove)
	{
		TokenToMove->SetActorLocation(Location, false, nullptr, ETeleportType::TeleportPhysics);
		TokenToMove->SetActorRotation(ActorRotation, ETeleportType::TeleportPhysics);
	}
}

void ABGGameplayGameModeBase::ToggleTokenLockInPlace(ABGToken* TokenToToggle, bool bLock)
{
	if (TokenToToggle)
	{
		TokenToToggle->ToggleLockTokenInPlace(bLock);
	}
}

void ABGGameplayGameModeBase::ToggleTokenPermissionsForPlayer(ABGPlayerState* PlayerStateToToggle,
                                                              ABGToken* TokenToToggle)
{
	if (PlayerStateToToggle && TokenToToggle)
	{
		TokenToToggle->PlayerHasPermissions(PlayerStateToToggle)
			? TokenToToggle->RemovePlayerFromPermissionsArray(PlayerStateToToggle)
			: TokenToToggle->AddPlayerToPermissionsArray(PlayerStateToToggle);
	}
}

void ABGGameplayGameModeBase::DestroyToken(ABGToken* TokenToDestroy)
{
	if (TokenToDestroy)
	{
		TokenToDestroy->Destroy();
	}
}

void ABGGameplayGameModeBase::SpawnStructureAtLocation(FVector const& Location, FName const& RowName)
{
	if (StructureDataBank)
	{
		auto const Row = StructureDataBank->FindRow<FBGStructureBank>(RowName, "");

		if (Row)
		{
			FTransform NewTransform{};
			NewTransform.SetLocation(Location);

			auto const StructureToSpawn = Cast<ABGSplineStructure>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
				this, Row->StructureClassReference, NewTransform,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
				this));

			UGameplayStatics::FinishSpawningActor(StructureToSpawn, NewTransform);
		}
	}
}

void ABGGameplayGameModeBase::SetStructurePhysicsAndCollision(ABGSplineStructure* StructureToModify,
                                                              bool const bGravityOn,
                                                              ECollisionEnabled::Type const CollisionType)
{
	if (StructureToModify)
	{
		StructureToModify->SetStructurePhysicsAndCollision(bGravityOn, CollisionType);
	}
}

void ABGGameplayGameModeBase::ModifyStructureLength(ABGSplineStructure* StructureToModify,
                                                    int const& PointIndex,
                                                    FVector const& NewLocation)
{
	if (StructureToModify)
	{
		StructureToModify->SetLocationOfSplinePoint(PointIndex, NewLocation);
		StructureToModify->AddMeshToSpline();
	}
}

void ABGGameplayGameModeBase::AddSplinePointToStructureSpline(ABGSplineStructure* StructureToModify,
                                                              FVector const& ClickLocation, int const& Index)
{
	if (StructureToModify)
	{
		StructureToModify->AddPointToSplineAtIndex(ClickLocation, Index);
	}
}

void ABGGameplayGameModeBase::MoveStructure(ABGSplineStructure* StructureToMove, FVector const& Location)
{
	if (StructureToMove)
	{
		StructureToMove->SetActorLocation(Location);
	}
}

void ABGGameplayGameModeBase::RemoveStructureInstanceAtIndex(ABGSplineStructure* StructureToModify, int const& Index)
{
	if (StructureToModify)
	{
		StructureToModify->GetInstancedStaticMeshComponent()->RemoveInstance(Index);
	}
}

void ABGGameplayGameModeBase::ResetStructure(ABGSplineStructure* StructureToReset)
{
	if (StructureToReset)
	{
		if (StructureToReset->GetSplineComponent()->GetNumberOfSplinePoints() > 2)
		{
			// clear all spline points except for indices 0 and 1
			for (int i{StructureToReset->GetSplineComponent()->GetNumberOfSplinePoints()}; i > 2; --i)
			{
				StructureToReset->GetSplineComponent()->RemoveSplinePoint(i - 1);
			}
		}

		// reset Spline Point 0 to actor's origin
		StructureToReset->GetSplineComponent()->SetLocationAtSplinePoint(
			0, StructureToReset->GetActorLocation(), ESplineCoordinateSpace::World, true);

		// reset Spline Point 1 to 105.f away the origin
		auto Location = StructureToReset->GetActorLocation();
		Location.X += 50.f;

		StructureToReset->GetSplineComponent()->SetLocationAtSplinePoint(
			1, Location, ESplineCoordinateSpace::World, true);

		StructureToReset->AddMeshToSpline();
	}
}

void ABGGameplayGameModeBase::DestroyStructure(ABGSplineStructure* StructureToDestroy)
{
	if (StructureToDestroy)
	{
		StructureToDestroy->Destroy();
	}
}

void ABGGameplayGameModeBase::SpawnNewBoard(int const& Zed, int const& X, int const& Y)
{
	for (auto Board : AllBoards)
	{
		if (Board->GetZedLevel() == Zed && Board->GetBoardTiles().Num() == 0)
		{
			Board->BuildBoard(Board->GetActorLocation(), X, Y);
		}
	}
}

void ABGGameplayGameModeBase::ToggleTileVisibility(ABGTile* TileToToggle)
{
	if (TileToToggle)
	{
		TileToToggle->ToggleTileVisibility(TileToToggle->GetStaticMeshComponent()->GetVisibleFlag());
	}
}

void ABGGameplayGameModeBase::ShrinkBoard(ABGBoard* BoardToShrink)
{
	if (BoardToShrink)
	{
		BoardToShrink->ShrinkBoard(BoardToShrink->GetBoardSize().X - 1, BoardToShrink->GetBoardSize().Y - 1);
	}
}

void ABGGameplayGameModeBase::GrowBoard(ABGBoard* BoardToGrow)
{
	if (BoardToGrow)
	{
		BoardToGrow->GrowBoard(BoardToGrow->GetBoardSize().X + 1, BoardToGrow->GetBoardSize().Y + 1);
	}
}

void ABGGameplayGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// populate AllBoards array with all ABGBoard actors in the world
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ABGBoard::StaticClass(), Actors);
	for (auto Actor : Actors)
	{
		auto Board = Cast<ABGBoard>(Actor);
		if (Board->GetZedLevel() == 0)
		{
			Board->BuildBoard(Board->GetActorLocation(), 10, 10);
		}
		AllBoards.AddUnique(Board);
	}
}
