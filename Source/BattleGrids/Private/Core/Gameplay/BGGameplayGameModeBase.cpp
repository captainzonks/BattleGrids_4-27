// © 2021 Matthew Barham. All Rights Reserved.

#include "Core/Gameplay/BGGameplayGameModeBase.h"

#include "Actors/BGBoard.h"
#include "Actors/BGSplineStructure.h"
#include "Actors/BGStructure.h"
#include "Actors/BGTile.h"
#include "Actors/BGToken.h"
#include "Components/SplineComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"


TArray<FName> ABGGameplayGameModeBase::GetRowNamesOfType(EBGObjectType const& ObjectType) const
{
	TArray<FName> Names{};

	for (auto const It : StaticMeshBank->GetRowMap())
	{
		if (auto const Struct = reinterpret_cast<FBGStaticMeshBank*>(It.Value))
		{
			if (Struct->ObjectType == ObjectType)
			{
				auto const NewString = Struct->StaticMeshName.ToString();
				auto const NewName = FName(*NewString);
				Names.AddUnique(NewName);
			}
		}
	}

	return Names;
}

void ABGGameplayGameModeBase::SpawnTokenAtLocation(FVector const& Location, FName const& MeshName,
                                                   FName const& MaterialName)
{
	if (StaticMeshBank && MaterialInstanceBank)
	{
		auto const ModelStaticMeshRow = StaticMeshBank->FindRow<FBGStaticMeshBank>(MeshName, "");
		auto const MaterialInstanceRow = MaterialInstanceBank->FindRow<FBGMaterialInstanceBank>(MaterialName, "");
		auto const TokenBaseStaticMeshRow = StaticMeshBank->FindRow<FBGStaticMeshBank>("Simple_Base_Round_01", "");

		if (ModelStaticMeshRow && MaterialInstanceRow && TokenBaseStaticMeshRow)
		{
			FTransform NewTransform{};
			NewTransform.SetLocation(Location);

			auto const TokenToSpawn = Cast<ABGToken>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
				this, ABGToken::StaticClass(), NewTransform,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn,
				this));

			TokenToSpawn->InitializeMeshAndMaterial(ModelStaticMeshRow->StaticMesh,
			                                        MaterialInstanceRow->MaterialInstance,
			                                        TokenBaseStaticMeshRow->StaticMesh);

			TokenToSpawn->GetTokenModelStaticMeshComponent()->SetSimulatePhysics(true);

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
		TokenToMove->SetActorLocation(Location, true, nullptr, ETeleportType::ResetPhysics);
		TokenToMove->SetActorRotation(ActorRotation, ETeleportType::ResetPhysics);
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

void ABGGameplayGameModeBase::SpawnSplineStructureAtLocation(FVector const& Location,
                                                             FName const& WallStaticMeshName,
                                                             FName const& WallMaskedMaterialInstanceName,
                                                             FName const& CornerStaticMeshName,
                                                             FName const& CornerMaskedMaterialInstanceName,
                                                             FName const& BaseStaticMeshName,
                                                             FName const& BaseMaterialInstanceName)
{
	if (StaticMeshBank && MaterialInstanceBank)
	{
		auto const WallStaticMeshRow = StaticMeshBank->FindRow<FBGStaticMeshBank>(WallStaticMeshName, "");
		auto const WallMaskedMaterialInstanceRow = MaterialInstanceBank->FindRow<FBGMaterialInstanceBank>(
			WallMaskedMaterialInstanceName, "");
		auto const CornerStaticMeshRow = StaticMeshBank->FindRow<FBGStaticMeshBank>(CornerStaticMeshName, "");
		auto const CornerMaskedMaterialInstanceRow = MaterialInstanceBank->FindRow<FBGMaterialInstanceBank>(
			CornerMaskedMaterialInstanceName, "");
		auto const BaseStaticMeshRow = StaticMeshBank->FindRow<FBGStaticMeshBank>(BaseStaticMeshName, "");
		auto const BaseMaterialRow = MaterialInstanceBank->FindRow<FBGMaterialInstanceBank>(
			BaseMaterialInstanceName, "");

		FTransform NewTransform{};
		NewTransform.SetLocation(Location);

		// Check to make sure we found all the data we need
		if (!WallStaticMeshRow || !WallMaskedMaterialInstanceRow
			|| !CornerStaticMeshRow || !CornerMaskedMaterialInstanceRow
			|| !BaseStaticMeshRow || !BaseMaterialRow)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot Spawn SplineStructure Due to Bad Row Name"))
			return;
		}

		FBGStructureInfo const NewStructureInfo(WallStaticMeshRow->StaticMesh,
		                                        WallMaskedMaterialInstanceRow->MaterialInstance,
		                                        CornerStaticMeshRow->StaticMesh,
		                                        CornerMaskedMaterialInstanceRow->MaterialInstance,
		                                        BaseStaticMeshRow->StaticMesh,
		                                        BaseMaterialRow->MaterialInstance,
		                                        NewTransform);

		auto const StructureToSpawn = Cast<ABGSplineStructure>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this, ABGSplineStructure::StaticClass(), NewTransform,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
			this));

		StructureToSpawn->SetStructureInfo(NewStructureInfo);

		UGameplayStatics::FinishSpawningActor(StructureToSpawn, NewTransform);
	}
}

void ABGGameplayGameModeBase::SetSplineStructurePhysicsAndCollision(ABGSplineStructure* StructureToModify,
                                                                    bool const bGravityOn,
                                                                    ECollisionEnabled::Type const CollisionType)
{
	if (StructureToModify)
	{
		StructureToModify->SetStructurePhysicsAndCollision(bGravityOn, CollisionType);
	}
}

void ABGGameplayGameModeBase::ModifySplineStructureLength(ABGSplineStructure* StructureToModify,
                                                          int const& PointIndex,
                                                          FVector const& NewLocation)
{
	if (StructureToModify)
	{
		StructureToModify->SetLocationOfSplinePoint(PointIndex, NewLocation);
		StructureToModify->UpdateStructureMesh();
	}
}

void ABGGameplayGameModeBase::AddSplinePointToSplineStructure(ABGSplineStructure* StructureToModify,
                                                              FVector const& ClickLocation, int const& Index)
{
	if (StructureToModify)
	{
		StructureToModify->AddPointToSplineAtIndex(ClickLocation, Index);
	}
}

void ABGGameplayGameModeBase::AddPointToSpline(ABGSplineStructure* StructureToModify, FVector const& ClickLocation,
                                               float const& Key)
{
	if (StructureToModify)
	{
		StructureToModify->AddPointToSpline(ClickLocation, Key);
	}
}

void ABGGameplayGameModeBase::MoveSplineStructure(ABGSplineStructure* StructureToMove, FVector const& Location)
{
	if (StructureToMove)
	{
		StructureToMove->SetActorLocation(Location);
	}
}

void ABGGameplayGameModeBase::ModifySplineStructureInstanceMeshAtIndex(ABGSplineStructure* StructureToModify,
                                                                       int const& Index,
                                                                       FString const& NewInstanceName,
                                                                       UStaticMesh* StaticMesh,
                                                                       UMaterialInstance* MaterialInstance,
                                                                       FString const& OldInstanceName)
{
	if (StructureToModify)
	{
		StructureToModify->ModifyInstanceMeshAtIndex(Index, NewInstanceName, StaticMesh,
		                                             MaterialInstance, OldInstanceName);
	}
}

void ABGGameplayGameModeBase::SpawnStructureActorAtSplineStructureIndex(ABGSplineStructure* StructureToModify,
                                                                        int const& Index,
                                                                        TSubclassOf<ABGStructure> StructureClassToSpawn,
                                                                        FString const& OldInstanceName)
{
	if (StructureToModify && StructureClassToSpawn)
	{
		FTransform SavedTransform;
		auto const Instance = StructureToModify->GetInstancedStaticMeshComponentByString(OldInstanceName);

		if (Instance)
		{
			Instance->GetInstanceTransform(Index, SavedTransform, true);
			SavedTransform.SetLocation(SavedTransform.GetLocation() + FVector(0.f, 0.f, 0.1f));

			StructureToModify->
				SpawnStructureActorAtIndex(Index, StructureClassToSpawn, SavedTransform, OldInstanceName);
			StructureToModify->RemoveInstanceMeshAtIndex(Index);
		}
	}
}

void ABGGameplayGameModeBase::DestroyStructureActor(ABGStructure* StructureToRemove)
{
	if (StructureToRemove)
	{
		if (!StructureToRemove->IsPendingKillOrUnreachable())
		{
			StructureToRemove->GetParentSplineStructure()->RemoveStructureFromArray(StructureToRemove);
			StructureToRemove->Destroy();
		}
	}
}

void ABGGameplayGameModeBase::RestoreInstanceMeshOnSplineStructure(ABGSplineStructure* StructureToModify,
                                                                   FTransform const& NewInstanceTransform,
                                                                   FString const& InstanceName)
{
	StructureToModify->RestoreInstanceMeshAtIndex(NewInstanceTransform, InstanceName);
}

void ABGGameplayGameModeBase::ResetSplineStructure(ABGSplineStructure* StructureToReset)
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

		StructureToReset->UpdateStructureMesh();
	}
}

void ABGGameplayGameModeBase::DestroySplineStructure(ABGSplineStructure* StructureToDestroy)
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
