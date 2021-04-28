// © 2021 Matthew Barham. All Rights Reserved.


#include "Components/BGSplineWallComponent.h"

#include "Actors/BGStructure.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Core/Gameplay/BGGameplayGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBGSplineWallComponent::UBGSplineWallComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bEditableWhenInherited = true;
	SetIsReplicatedByDefault(true);

	bLocked = false;
}

// Called when the game starts
void UBGSplineWallComponent::BeginPlay()
{
	Super::BeginPlay();

	SplineComponent = NewObject<USplineComponent>(
		GetOwner(),
		USplineComponent::StaticClass(),
		TEXT("Spline Component"));
	
	if (SplineComponent)
	{
		SplineComponent->SetupAttachment(GetOwner()->GetRootComponent());
		SplineComponent->RegisterComponent();
		SplineComponent->CreationMethod = EComponentCreationMethod::Instance;
		SplineComponent->SetIsReplicated(true);
		SplineComponent->SetLocationAtSplinePoint(1, FVector(50.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);
		SplineComponent->SetSplinePointType(0, ESplinePointType::Linear);
		SplineComponent->SetSplinePointType(1, ESplinePointType::Linear);
		UpdateSplineStructureMesh();
	}
}

void UBGSplineWallComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBGSplineWallComponent, StructureData)
	DOREPLIFETIME(UBGSplineWallComponent, SpawnedStructures)
	DOREPLIFETIME(UBGSplineWallComponent, UniqueCollisionNumber)
	DOREPLIFETIME(UBGSplineWallComponent, UniqueInstanceNumber)
	DOREPLIFETIME(UBGSplineWallComponent, bLocked)
}

void UBGSplineWallComponent::RemoveInstanceMeshAtIndex_Implementation(int const& Index, FString const& InstanceName)
{
	// UE_LOG(LogTemp, Warning, TEXT("Removing Instance at Index: %i"), Index);

	auto Instance = GetInstancedStaticMeshComponentByTag(InstanceName);

	if (Instance)
	{
		Instance->RemoveInstance(Index);
		CollisionBoxes[Index]->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		CollisionBoxes[Index]->SetCanEverAffectNavigation(false);
		DisabledCollisionBoxes.Add(CollisionBoxes[Index]);
	}
}

void UBGSplineWallComponent::RestoreInstanceMeshAtIndex_Implementation(int const& Index, FTransform const& Transform,
                                                                       FString const& InstanceName)
{
	auto Instance = GetInstancedStaticMeshComponentByTag(InstanceName);

	if (Instance)
	{
		auto const NewTransform = Transform.GetRelativeTransform(GetOwner()->GetActorTransform());
		Instance->AddInstance(NewTransform);

		// Turn BoxCollision back on
		CollisionBoxes[Index]->SetCollisionProfileName("StructureBoxCollision");
		CollisionBoxes[Index]->SetCanEverAffectNavigation(true);
		DisabledCollisionBoxes.RemoveSingle(CollisionBoxes[Index]);
	}
}

void UBGSplineWallComponent::SpawnStructureActorAtIndex_Implementation(int const& Index,
                                                                       TSubclassOf<ABGStructure> StructureClassToSpawn,
                                                                       FTransform const& SpawnTransform,
                                                                       FString const& OldInstanceName)
{
	if (StructureClassToSpawn)
	{
		// Prepare a Structure to spawn using the passed parameter
		ABGStructure* StructureToSpawn = GetWorld()->SpawnActorDeferred<ABGStructure>(
			StructureClassToSpawn->GetDefaultObject()->GetClass(), SpawnTransform);
		StructureToSpawn->SetParentSplineWallComponent(this);
		StructureToSpawn->SetIndexOnSplineStructure(Index);

		auto const StructureContextMenuClass = GetWorld()->GetAuthGameMode<ABGGameplayGameModeBase>()->
		                                                   GetStructureContextMenuClass();
		if (StructureContextMenuClass.GetDefaultObject())
		{
			StructureToSpawn->SetWidgetComponentClass(StructureContextMenuClass);
		}


		// Spawn the new Structure and add it to the SplineStructure's array
		if (auto Structure = Cast<ABGStructure>(UGameplayStatics::FinishSpawningActor(StructureToSpawn, SpawnTransform))
		)
		{
			SpawnedStructures.Add(Structure);
			Structure->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void UBGSplineWallComponent::ModifyInstanceMeshAtIndex_Implementation(int const& Index, FString const& NewInstanceName,
                                                                      UStaticMesh* StaticMesh,
                                                                      UMaterialInstance* MaterialInstance,
                                                                      FString const& OldInstanceName)
{
	if (StaticMesh && MaterialInstance)
	{
		FTransform SavedTransform;

		auto const OldInstance = GetInstancedStaticMeshComponentByTag(OldInstanceName);

		// Find the Old Instance (such as "WallInstance"), save its transform and then remove it
		if (OldInstance)
		{
			OldInstance->GetInstanceTransform(Index, SavedTransform, false);
			SavedTransform.SetLocation(SavedTransform.GetLocation() + FVector(0.f, 0.f, 0.1f));

			RemoveInstanceMeshAtIndex(Index);

			// CollisionBoxes[Index]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// Check if we already have an InstancedStaticMeshComponent with the new name (such as "DoorInstance")
		// If we do, add to that one instead of making a new one
		for (auto InstanceComponent : GetOwner()->GetInstanceComponents())
		{
			if (InstanceComponent && InstanceComponent->GetName().Equals(NewInstanceName))
			{
				if (auto CastInstance = Cast<UInstancedStaticMeshComponent>(InstanceComponent))
				{
					CastInstance->AddInstance(SavedTransform);
					return;
				}
			}
		}

		// Otherwise...make a new InstancedStaticMeshComponent with the new InstanceName
		auto NewInstance = CreateInstancedStaticMeshComponent(NewInstanceName, StaticMesh,
		                                                      MaterialInstance);
		NewInstance->AddInstance(SavedTransform);
	}
}

void UBGSplineWallComponent::UpdateSplineStructureMesh_Implementation(int const& PointIndex)
{
	CleanupInstanceComponents(PointIndex);

	if (StructureData.WallStaticMesh && StructureData.WallMaskedMaterialInstance)
	{
		UpdateWallMesh();
	}

	if (StructureData.CornerStaticMesh && StructureData.CornerMaskedMaterialInstance)
	{
		UpdateCornerMesh();
	}

	GetOwner()->UpdateComponentTransforms();
}

void UBGSplineWallComponent::SetLocationOfSplinePoint_Implementation(int const& PointIndex, FVector const& NewLocation)
{
	SplineComponent->SetLocationAtSplinePoint(PointIndex, NewLocation, ESplineCoordinateSpace::World, true);
}

void UBGSplineWallComponent::AddPointToSpline_Implementation(FVector const& Location, float const& Key)
{
	FSplinePoint NewSplinePoint;
	NewSplinePoint.Type = ESplinePointType::Linear;
	NewSplinePoint.Position = Location;
	NewSplinePoint.ArriveTangent = FVector::ZeroVector;
	NewSplinePoint.LeaveTangent = FVector::ZeroVector;
	NewSplinePoint.Rotation = FRotator::ZeroRotator;
	NewSplinePoint.InputKey = Key;
	SplineComponent->AddPoint(NewSplinePoint);
	UpdateSplineStructureMesh();
}

void UBGSplineWallComponent::AddPointToSplineAtIndex_Implementation(FVector const& Location, int const& Index)
{
	SplineComponent->AddSplinePointAtIndex(Location, Index, ESplineCoordinateSpace::World, true);
	SplineComponent->SetSplinePointType(Index, ESplinePointType::Linear, true);
}

void UBGSplineWallComponent::ToggleLockStructureInPlace_Implementation(bool const bNewLocked)
{
	SetLocked(bNewLocked);
}

void UBGSplineWallComponent::RemoveStructureFromArray(ABGStructure* StructureToRemove)
{
	SpawnedStructures.RemoveSingle(StructureToRemove);
}

UInstancedStaticMeshComponent* UBGSplineWallComponent::GetInstancedStaticMeshComponentByTag(FString const& Tag) const
{
	for (auto Instance : GetOwner()->GetInstanceComponents())
	{
		if (Instance && !Instance->IsBeingDestroyed() && Instance->ComponentHasTag(FName(Tag)))
		{
			if (auto const CastInstance = Cast<UInstancedStaticMeshComponent>(Instance))
			{
				return CastInstance;
			}
		}
	}
	return nullptr;
}

void UBGSplineWallComponent::SetStructureInfo(FBGStructureInfo const& NewStructureInfo)
{
	StructureData = NewStructureInfo;
}


void UBGSplineWallComponent::CleanupInstanceComponents(int const& PointIndex)
{
	auto CurrentInstanceComponents = GetOwner()->GetInstanceComponents();

	// UE_LOG(LogTemp, Warning, TEXT("Removing all instances"))

	if (CurrentInstanceComponents.Num() > 0)
	{
		for (auto Instance : CurrentInstanceComponents)
		{
			if (Instance && !Instance->IsPendingKillOrUnreachable())
				Instance->DestroyComponent();
		}
		CurrentInstanceComponents.Empty();
	}

	if (CollisionBoxes.Num() > 0)
	{
		for (auto Box : CollisionBoxes)
		{
			if (Box && !Box->IsPendingKillOrUnreachable())
			{
				Box->DestroyComponent();
			}
		}
		CollisionBoxes.Empty();
	}

	if (DisabledCollisionBoxes.Num() > 0)
	{
		for (auto Box : DisabledCollisionBoxes)
		{
			if (Box && !Box->IsPendingKillOrUnreachable())
			{
				Box->DestroyComponent();
			}
		}
		DisabledCollisionBoxes.Empty();
	}

	if (SpawnedStructures.Num() > 0)
	{
		for (auto Structure : SpawnedStructures)
		{
			if (Structure && !Structure->IsPendingKillOrUnreachable())
			{
				Structure->Destroy();
			}
		}
		SpawnedStructures.Empty();
	}
}

void UBGSplineWallComponent::UpdateWallMesh()
{
	auto WallInstancedComponent = GetInstancedStaticMeshComponentByTag("WallInstance");

	if (!WallInstancedComponent->IsValidLowLevel() || WallInstancedComponent->IsPendingKillOrUnreachable())
	{
		// UE_LOG(LogTemp, Warning, TEXT("UpdateWallMesh(): Creating new CornerInstance"))

		WallInstancedComponent = CreateInstancedStaticMeshComponent(
			"WallInstance", StructureData.WallStaticMesh,
			StructureData.WallMaskedMaterialInstance);
		WallInstancedComponent->SetCollisionProfileName("Structure");
	}

	auto BaseInstancedComponent = GetInstancedStaticMeshComponentByTag("WallBaseInstance");

	if (!BaseInstancedComponent->IsValidLowLevel() || WallInstancedComponent->IsPendingKillOrUnreachable())
	{
		// UE_LOG(LogTemp, Warning, TEXT("UpdateWallMesh(): Creating new WallBaseInstance"))

		BaseInstancedComponent = CreateInstancedStaticMeshComponent(
			"WallBaseInstance", StructureData.BaseStaticMesh,
			StructureData.BaseMaterialInstance);
		BaseInstancedComponent->SetCollisionProfileName("Base");
	}

	int LoopCount{1};

	if (!SplineComponent) return;
	for (int i{}; i <= SplineComponent->GetNumberOfSplinePoints() - 2; ++i)
	{
		auto LocationOne = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		auto LocationTwo = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		auto const Distance = FVector::Dist(LocationOne, LocationTwo);
		// TODO: don't hardcode repeat-width (eventually remove and replace with globally set variable)
		auto const RepeatWidth{100.f};

		int const Integral = FMath::TruncToInt(Distance / 100.f) - 1;
		// UE_LOG(LogTemp, Warning, TEXT("Integral: %i"), Integral)

		/** Builds base meshes between Spline Points */
		for (int j{}; j <= Integral; ++j)
		{
			FTransform BaseTransform{};

			const auto SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
				LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);
			const auto SplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(
				LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);

			FVector BaseLocation{};
			BaseLocation.X = SplineLocation.X;
			BaseLocation.Y = SplineLocation.Y;
			BaseLocation.Z = SplineLocation.Z;

			FRotator BaseRotation{};
			BaseRotation.Pitch = SplineRotation.Pitch + StructureData.Transform.GetRotation().Rotator().Pitch;
			BaseRotation.Yaw = SplineRotation.Yaw + StructureData.Transform.GetRotation().Rotator().Yaw;
			BaseRotation.Roll = SplineRotation.Roll + StructureData.Transform.GetRotation().Rotator().Roll;

			BaseTransform.SetLocation(BaseLocation);
			BaseTransform.SetRotation(FQuat(BaseRotation));
			BaseTransform.SetScale3D(StructureData.Transform.GetScale3D());


			++LoopCount;
			BaseInstancedComponent->AddInstance(BaseTransform);

			if (BaseTransform.GetLocation() == LocationOne || BaseTransform.GetLocation() == LocationTwo)
			{
				// UE_LOG(LogTemp, Warning, TEXT("Corner Point, Skipping Wall Mesh"))
				continue;
			}

			FTransform StructureTransform{BaseTransform};

			int RandomNumber = FMath::RandRange(0, 10);
			StructureTransform.SetRotation(
				FQuat(StructureTransform.Rotator().Add(0.f, RandomNumber < 4 ? 0.f : 180.f, 0.f)));
			StructureTransform.SetLocation(StructureTransform.GetLocation() + FVector(0.f, 0.f, 0.1f));

			WallInstancedComponent->AddInstance(StructureTransform);

			// Create a Box Collision for each instance
			auto BoxCollision = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(),
			                                             FName(TEXT(
				                                             "Collision" + FString::FromInt(UniqueCollisionNumber++))),
			                                             RF_Transient);
			BoxCollision->RegisterComponent();
			BoxCollision->SetCollisionProfileName("StructureBoxCollision");
			BoxCollision->AttachToComponent(SplineComponent,
			                                FAttachmentTransformRules::SnapToTargetIncludingScale);
			BoxCollision->SetRelativeLocation(BaseTransform.GetLocation() + FVector(
				0.f, 0.f, WallInstancedComponent->GetStaticMesh()->
				                                  GetBounds().BoxExtent.Z));
			BoxCollision->SetRelativeRotation(BaseRotation);
			FVector BoxBounds = WallInstancedComponent->GetStaticMesh()->GetBounds().Origin
				+ WallInstancedComponent->GetStaticMesh()->GetBounds().BoxExtent
				+ FVector(0.5f, 0.5f, 0.05f);
			BoxBounds.Z /= 2;
			BoxCollision->SetBoxExtent(BoxBounds);
			CollisionBoxes.AddUnique(BoxCollision);

			// TODO: Fix overlaps for component
			// BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABGSplineStructure::OnOverlapBegin);
			// BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ABGSplineStructure::OnOverlapEnd);
		}
	}
}

void UBGSplineWallComponent::UpdateCornerMesh()
{
	auto CornerInstancedComponent = GetInstancedStaticMeshComponentByTag("CornerInstance");

	if (!CornerInstancedComponent->IsValidLowLevel() || CornerInstancedComponent->IsPendingKillOrUnreachable())
	{
		// UE_LOG(LogTemp, Warning, TEXT("UpdateCornerMesh(): Creating new CornerInstance"))
		CornerInstancedComponent = CreateInstancedStaticMeshComponent(
			"CornerInstance", StructureData.CornerStaticMesh,
			StructureData.CornerMaskedMaterialInstance);
		CornerInstancedComponent->SetCollisionProfileName("Structure");
	}

	auto BaseInstancedComponent = GetInstancedStaticMeshComponentByTag("CornerBaseInstance");

	if (!BaseInstancedComponent->IsValidLowLevel() || BaseInstancedComponent->IsPendingKillOrUnreachable())
	{
		// UE_LOG(LogTemp, Warning, TEXT("UpdateCornerMesh(): Creating new CornerBaseInstance"))
		BaseInstancedComponent = CreateInstancedStaticMeshComponent(
			"CornerBaseInstance", StructureData.BaseStaticMesh,
			StructureData.BaseMaterialInstance);
		BaseInstancedComponent->SetCollisionProfileName("Base");
	}

	for (int i{}; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
	{
		auto SplinePointTransform = SplineComponent->GetTransformAtSplinePoint(i, ESplineCoordinateSpace::Local, false);

		CornerInstancedComponent->AddInstance(SplinePointTransform);

		BaseInstancedComponent->AddInstance(SplinePointTransform);
	}
}

UInstancedStaticMeshComponent* UBGSplineWallComponent::CreateInstancedStaticMeshComponent(FString const& InstanceTag,
	UStaticMesh* StaticMesh, UMaterialInstance* MaterialInstance)
{
	auto NewInstancedStaticMeshComponent = NewObject<UInstancedStaticMeshComponent>(
		this, *(InstanceTag + FString::FromInt(UniqueInstanceNumber++)));
	NewInstancedStaticMeshComponent->RegisterComponent();
	NewInstancedStaticMeshComponent->SetIsReplicated(true);
	NewInstancedStaticMeshComponent->AttachToComponent(SplineComponent,
	                                                   FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewInstancedStaticMeshComponent->SetFlags(RF_Transactional);
	// Use tag for keeping track of Instance type (i.e. Wall, Corner, Base, etc.)
	NewInstancedStaticMeshComponent->ComponentTags.Add(FName(InstanceTag));

	if (StaticMesh)
	{
		NewInstancedStaticMeshComponent->SetStaticMesh(StaticMesh);
	}
	if (MaterialInstance)
	{
		NewInstancedStaticMeshComponent->SetMaterial(0, MaterialInstance);
	}

	GetOwner()->AddInstanceComponent(NewInstancedStaticMeshComponent);

	return NewInstancedStaticMeshComponent;
}
