// © 2021 Matthew Barham. All Rights Reserved.

#include "Core/Gameplay/BGGameplayGameModeBase.h"


#include "Actors/BGBoard.h"
#include "Actors/BGSplineStructure.h"
#include "Actors/BGTile.h"
#include "Actors/BGToken.h"
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

void ABGGameplayGameModeBase::SetTokenPhysicsAndCollision(ABGToken* TokenToMove, bool const bPhysicsOn,
                                                          bool const bGravityOn,
                                                          ECollisionEnabled::Type const CollisionType)
{
	if (TokenToMove)
	{
		TokenToMove->SetTokenPhysicsAndCollision(bPhysicsOn, bGravityOn, CollisionType);
	}
}

void ABGGameplayGameModeBase::MoveTokenToLocation(ABGToken* TokenToMove, AActor* TargetActor, bool const& bHolding,
                                                  FRotator const& ActorRotation)
{
	if (TokenToMove && TargetActor)
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
