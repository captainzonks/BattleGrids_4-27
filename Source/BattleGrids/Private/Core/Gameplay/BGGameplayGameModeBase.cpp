// © 2021 Matthew Barham. All Rights Reserved.

#include "Core/Gameplay/BGGameplayGameModeBase.h"
#include "AIController.h"
#include "Actors/BGBoard.h"
#include "Actors/BGSplineStructure.h"
#include "Actors/BGStructure.h"
#include "Actors/BGTile.h"
#include "Characters/BGToken.h"
#include "Components/BGSplineWallComponent.h"
#include "Components/BGTileComponent.h"
#include "Components/SplineComponent.h"
#include "Core/Gameplay/BGGameplayGameStateBase.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

ABGGameplayGameModeBase::ABGGameplayGameModeBase()
{
}

void ABGGameplayGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ABGGameplayGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABGGameplayGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ABGGameplayGameModeBase::HandleSeamlessTravelPlayer(AController*& C)
{
	Super::HandleSeamlessTravelPlayer(C);

	UE_LOG(LogTemp, Warning, TEXT("Handle Seamless Travel Player"))
}

ABGActor* ABGGameplayGameModeBase::CreateNewActor(EBGActorType const& ActorType)
{
	switch (ActorType)
	{
	case EBGActorType::None: break;

	case EBGActorType::Structure:
		return CreateNewSplineWallActor();

	case EBGActorType::Tile:
		return CreateNewTileActor();

	case EBGActorType::Board: break;

	case EBGActorType::Light: break;

	default: ;
	}
	return nullptr;
}

ABGActor* ABGGameplayGameModeBase::CreateNewSplineWallActor() const
{
	auto const NewStructureActor = UGameplayStatics::BeginDeferredActorSpawnFromClass(
		this, ABGActor::StaticClass(), FTransform::Identity);
	auto CastNewStructure = Cast<ABGActor>(NewStructureActor);
	if (CastNewStructure)
	{
		CastNewStructure->SetActorType(EBGActorType::Structure);
		auto SplineWallComponent = NewStructureActor->AddComponentByClass(
			UBGSplineWallComponent::StaticClass(),
			false,
			FTransform::Identity,
			false);
		SplineWallComponent->CreationMethod = EComponentCreationMethod::Instance;
		SplineWallComponent->Rename(TEXT("Spline Wall Component"));

		return CastNewStructure;
	}

	return nullptr;
}

ABGActor* ABGGameplayGameModeBase::CreateNewTileActor() const
{
	auto const NewTileActor = UGameplayStatics::BeginDeferredActorSpawnFromClass(
		this, ABGActor::StaticClass(), FTransform::Identity);
	auto CastNewTile = Cast<ABGActor>(NewTileActor);
	if (CastNewTile)
	{
		CastNewTile->SetActorType(EBGActorType::Tile);
		auto TileComponent = NewTileActor->AddComponentByClass(
			UBGTileComponent::StaticClass(),
			false,
			FTransform::Identity,
			false);
		TileComponent->CreationMethod = EComponentCreationMethod::Instance;
		TileComponent->Rename(TEXT("Tile Component"));

		return CastNewTile;
	}
	return nullptr;
}

TArray<FName> ABGGameplayGameModeBase::GetRowNamesOfType(EBGClassCategory const& ClassCategory) const
{
	TArray<FName> Names{};

	for (auto const It : StaticMeshBank->GetRowMap())
	{
		if (auto const Struct = reinterpret_cast<FBGStaticMeshBank*>(It.Value))
		{
			if (Struct->ClassCategory == ClassCategory)
			{
				auto const NewString = Struct->StaticMeshName.ToString();
				auto const NewName = FName(*NewString);
				Names.AddUnique(NewName);
			}
		}
	}

	return Names;
}

void ABGGameplayGameModeBase::DestroyGameActor(AActor* ActorToDestroy)
{
	if (ActorToDestroy)
	{
		ActorToDestroy->Destroy();
	}
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

			// TokenToSpawn->SetWidgetComponentClass(TokenContextMenuClass);

			TokenToSpawn->FinishSpawning(NewTransform);
		}
	}
}

void ABGGameplayGameModeBase::MoveTokenToLocation(ABGToken* TokenToMove, FVector const& Location,
                                                  FRotator const& ActorRotation)
{
	if (TokenToMove)
	{
		if (TokenToMove->GetIsTokenLocked())
		{
			return;
		}

		TokenToMove->SetActorRotation(ActorRotation, ETeleportType::ResetPhysics);
		TokenToMove->GetCharacterAIController()->MoveToLocation(
			Location,
			.1f,
			false,
			true,
			true,
			true,
			nullptr,
			false);
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

// void ABGGameplayGameModeBase::DestroyToken(ABGToken* TokenToDestroy)
// {
// 	if (TokenToDestroy)
// 	{
// 		TokenToDestroy->Destroy();
// 	}
// }

void ABGGameplayGameModeBase::CreateStructureInfo(FBGStructureInfo& OutStructureInfo,
                                                  FName const& WallStaticMeshName,
                                                  FName const& WallMaskedMaterialInstanceName,
                                                  FName const& CornerStaticMeshName,
                                                  FName const& CornerMaskedMaterialInstanceName,
                                                  FName const& BaseStaticMeshName,
                                                  FName const& BaseMaterialInstanceName,
                                                  FVector const& Location)
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
			UE_LOG(LogTemp, Warning, TEXT("Bad Row Name"))
			return;
		}

		FBGStructureInfo const NewStructureInfo(WallStaticMeshRow->StaticMesh,
		                                        WallMaskedMaterialInstanceRow->MaterialInstance,
		                                        CornerStaticMeshRow->StaticMesh,
		                                        CornerMaskedMaterialInstanceRow->MaterialInstance,
		                                        BaseStaticMeshRow->StaticMesh,
		                                        BaseMaterialRow->MaterialInstance,
		                                        NewTransform);

		OutStructureInfo = NewStructureInfo;
	}
}

void ABGGameplayGameModeBase::ModifySplineStructureLength(UBGSplineWallComponent* InSplineComponent,
                                                          int const& PointIndex,
                                                          FVector const& NewLocation)
{
	if (InSplineComponent)
	{
		InSplineComponent->SetLocationOfSplinePoint(PointIndex, NewLocation);
		InSplineComponent->UpdateSplineStructureMesh(PointIndex);
	}
}

void ABGGameplayGameModeBase::AddSplinePointToSplineStructure(UBGSplineWallComponent* InSplineComponent,
                                                              FVector const& ClickLocation, int const& Index)
{
	if (InSplineComponent)
	{
		InSplineComponent->AddPointToSplineAtIndex(ClickLocation, Index);
	}
}

void ABGGameplayGameModeBase::AddPointToSpline(UBGSplineWallComponent* InSplineComponent, FVector const& ClickLocation,
                                               float const& Key)
{
	if (InSplineComponent)
	{
		InSplineComponent->AddPointToSpline(ClickLocation, Key);
	}
}

// void ABGGameplayGameModeBase::MoveSplineStructure(ABGSplineStructure* StructureToMove, FVector const& Location)
// {
// 	if (StructureToMove)
// 	{
// 		StructureToMove->SetActorLocation(Location);
// 	}
// }

void ABGGameplayGameModeBase::ModifySplineStructureInstanceMeshAtIndex(UBGSplineWallComponent* InSplineComponent,
                                                                       int const& Index,
                                                                       FString const& NewInstanceName,
                                                                       UStaticMesh* StaticMesh,
                                                                       UMaterialInstance* MaterialInstance,
                                                                       FString const& OldInstanceName)
{
	if (InSplineComponent)
	{
		InSplineComponent->ModifyInstanceMeshAtIndex(Index, NewInstanceName, StaticMesh,
		                                             MaterialInstance, OldInstanceName);
	}
}

void ABGGameplayGameModeBase::SpawnStructureActorAtSplineStructureIndex(UBGSplineWallComponent* InSplineComponent,
                                                                        int const& Index,
                                                                        TSubclassOf<ABGStructure> StructureClassToSpawn,
                                                                        FString const& OldInstanceName)
{
	if (InSplineComponent && StructureClassToSpawn)
	{
		auto const Instance = InSplineComponent->GetInstancedStaticMeshComponentByTag(OldInstanceName);

		if (Instance)
		{
			FTransform SavedTransform;
			Instance->GetInstanceTransform(Index, SavedTransform, true);
			SavedTransform.SetLocation(SavedTransform.GetLocation() + FVector(0.f, 0.f, 0.1f));

			InSplineComponent->
				SpawnStructureActorAtIndex(Index, StructureClassToSpawn, SavedTransform, OldInstanceName);
			InSplineComponent->RemoveInstanceMeshAtIndex(Index);
		}
	}
}

void ABGGameplayGameModeBase::DestroyStructureActor(ABGStructure* StructureToRemove)
{
	if (StructureToRemove)
	{
		if (!StructureToRemove->IsPendingKillOrUnreachable())
		{
			StructureToRemove->GetParentSplineWallComponent()->RemoveStructureFromArray(StructureToRemove);
			StructureToRemove->Destroy();
		}
	}
}

void ABGGameplayGameModeBase::RestoreInstanceMeshOnSplineStructure(UBGSplineWallComponent* InSplineComponent,
                                                                   int const& Index,
                                                                   FTransform const& NewInstanceTransform,
                                                                   FString const& InstanceName)
{
	InSplineComponent->RestoreInstanceMeshAtIndex(Index, NewInstanceTransform, InstanceName);
}

void ABGGameplayGameModeBase::ResetSplineStructure(UBGSplineWallComponent* InSplineComponent)
{
	if (InSplineComponent)
	{
		if (InSplineComponent->GetSplineComponent()->GetNumberOfSplinePoints() > 2)
		{
			// clear all spline points except for indices 0 and 1
			for (int i{InSplineComponent->GetSplineComponent()->GetNumberOfSplinePoints()}; i > 2; --i)
			{
				InSplineComponent->GetSplineComponent()->RemoveSplinePoint(i - 1);
			}
		}

		auto SplineActorLocation = InSplineComponent->GetOwner()->GetActorLocation();

		// reset Spline Point 0 to actor's origin
		InSplineComponent->GetSplineComponent()->SetLocationAtSplinePoint(
			0, SplineActorLocation, ESplineCoordinateSpace::World, true);

		// reset Spline Point 1 to 105.f away the origin
		SplineActorLocation.X += 50.f;

		InSplineComponent->GetSplineComponent()->SetLocationAtSplinePoint(
			1, SplineActorLocation, ESplineCoordinateSpace::World, true);

		InSplineComponent->UpdateSplineStructureMesh();
	}
}

// void ABGGameplayGameModeBase::DestroySplineStructure(ABGSplineStructure* StructureToDestroy)
// {
// 	if (StructureToDestroy)
// 	{
// 		StructureToDestroy->Destroy();
// 	}
// }

void ABGGameplayGameModeBase::SpawnNewBoard(int const& Zed, int const& X, int const& Y) const
{
	auto const BGGameplayGameState = GetGameState<ABGGameplayGameStateBase>();
	if (BGGameplayGameState)
	{
		for (auto Board : BGGameplayGameState->GetAllBoards())
		{
			if (Board->GetZedLevel() == Zed && Board->GetBoardTiles().Num() == 0)
			{
				Board->BuildBoard(Board->GetActorLocation(), X, Y);
			}
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

void ABGGameplayGameModeBase::SpawnNewActor(TSubclassOf<ABGActor> const ActorToSpawn) const
{
	if (ActorToSpawn.GetDefaultObject())
	{
		auto NewActor = Cast<ABGActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this, ActorToSpawn, FTransform::Identity));

		NewActor->FinishSpawning(FTransform::Identity);
	}
}

void ABGGameplayGameModeBase::MoveActorToLocation(AActor* ActorToMove, FTransform const& NewTransform)
{
	if (ActorToMove)
	{
		ActorToMove->SetActorTransform(NewTransform);
	}
}
