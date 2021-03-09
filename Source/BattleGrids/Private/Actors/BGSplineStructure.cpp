// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGSplineStructure.h"
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABGSplineStructure::ABGSplineStructure()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// Need to have Scene Component as root, so that everything moves properly!
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(RootComponent);
	SplineComponent->SetIsReplicated(true);
	SplineComponent->SetLocationAtSplinePoint(1, FVector(50.f, 0.f, 0.f), ESplineCoordinateSpace::Local, true);
	SplineComponent->SetSplinePointType(0, ESplinePointType::Linear);
	SplineComponent->SetSplinePointType(1, ESplinePointType::Linear);

	bLocked = false;
}

void ABGSplineStructure::ModifyInstanceMeshAtIndex_Implementation(int const& Index,
                                                                  FString const& NewInstanceName,
                                                                  UStaticMesh* StaticMesh,
                                                                  UMaterialInstance* MaterialInstance,
                                                                  FString const& OldInstanceName)
{
	if (StaticMesh && MaterialInstance)
	{
		FTransform SavedTransform;

		auto OldInstance = GetInstancedStaticMeshComponentByString(OldInstanceName);
		if (OldInstance)
		{
			OldInstance->GetInstanceTransform(Index, SavedTransform, false);
			OldInstance->RemoveInstance(Index);
		}

		for (auto InstanceComponent : GetInstanceComponents())
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

		auto NewInstance = CreateInstancedStaticMeshComponent(NewInstanceName, StaticMesh,
		                                                      MaterialInstance);
		NewInstance->AddInstance(SavedTransform);
	}
}

void ABGSplineStructure::SetStructurePhysicsAndCollision_Implementation(bool const bGravityOn,
                                                                        ECollisionEnabled::Type const CollisionType)
{
	auto CurrentInstanceComponents = this->GetInstanceComponents();
	if (CurrentInstanceComponents.Num() > 0)
	{
		for (auto Instance : CurrentInstanceComponents)
		{
			if (auto InstancedCast = Cast<UInstancedStaticMeshComponent>(Instance))
			{
				InstancedCast->SetEnableGravity(bGravityOn);
				InstancedCast->SetCollisionEnabled(CollisionType);
				CollisionType == ECollisionEnabled::Type::QueryAndPhysics
					? InstancedCast->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block)
					: InstancedCast->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			}
		}
	}
}

void ABGSplineStructure::UpdateStructureMesh_Implementation()
{
	CleanupInstanceComponents();

	if (StructureData.PrimaryStaticMesh && StructureData.PrimaryMaterialInstance)
	{
		UpdateWallMesh();
	}

	if (StructureData.SecondaryStaticMesh && StructureData.SecondaryMaterialInstance)
	{
		UpdateCornerMesh();
	}

	SetStructurePhysicsAndCollision(true, ECollisionEnabled::QueryAndPhysics);
}

void ABGSplineStructure::SetLocationOfSplinePoint_Implementation(int const& PointIndex, FVector const& NewLocation)
{
	SplineComponent->SetLocationAtSplinePoint(PointIndex, NewLocation, ESplineCoordinateSpace::World, true);
}

void ABGSplineStructure::AddPointToSplineAtIndex_Implementation(FVector const& Location, int const& Index)
{
	SplineComponent->AddSplinePointAtIndex(Location, Index, ESplineCoordinateSpace::World, true);
	SplineComponent->SetSplinePointType(Index, ESplinePointType::Linear, true);
}

// Called when the game starts or when spawned
void ABGSplineStructure::BeginPlay()
{
	Super::BeginPlay();

	SetReplicatingMovement(true);
}

void ABGSplineStructure::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateStructureMesh();
}

void ABGSplineStructure::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGSplineStructure, StructureData)
	DOREPLIFETIME(ABGSplineStructure, bLocked)
}

void ABGSplineStructure::CleanupInstanceComponents() const
{
	auto CurrentInstanceComponents = GetInstanceComponents();

	if (CurrentInstanceComponents.Num() > 0)
	{
		for (auto Instance : CurrentInstanceComponents)
		{
			if (Instance && !Instance->IsPendingKillOrUnreachable())
				Instance->DestroyComponent();
		}
	}
}

void ABGSplineStructure::UpdateWallMesh()
{
	auto WallInstancedComponent = CreateInstancedStaticMeshComponent(
		"WallInstance", StructureData.PrimaryStaticMesh,
		StructureData.PrimaryMaterialInstance);

	int LoopCount{};

	for (int i{}; i <= SplineComponent->GetNumberOfSplinePoints() - 2; ++i)
	{
		auto LocationOne = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		auto LocationTwo = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		auto const Distance = FVector::Dist(LocationOne, LocationTwo);
		// don't hardcode repeat-width (eventually remove and replace with globally set variable)
		auto const RepeatWidth{100.f};

		int const Integral = FMath::TruncToInt(Distance / 100.f);

		// Builds wall meshes between Spline Points
		for (int j{}; j <= Integral; ++j)
		{
			FTransform NewTransform{};

			const auto SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
				LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);
			const auto SplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(
				LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);

			FVector NewLocation{};
			NewLocation.X = SplineLocation.X;
			NewLocation.Y = SplineLocation.Y;
			NewLocation.Z = SplineLocation.Z;

			FRotator NewRotation{};
			NewRotation.Pitch = SplineRotation.Pitch + StructureData.Transform.GetRotation().Rotator().Pitch;
			NewRotation.Yaw = SplineRotation.Yaw + StructureData.Transform.GetRotation().Rotator().Yaw;
			NewRotation.Roll = SplineRotation.Roll + StructureData.Transform.GetRotation().Rotator().Roll;

			NewTransform.SetLocation(NewLocation);
			NewTransform.SetRotation(FQuat(NewRotation));
			NewTransform.SetScale3D(StructureData.Transform.GetScale3D());

			WallInstancedComponent->AddInstance(NewTransform);
			++LoopCount;
		}
	}
}

void ABGSplineStructure::UpdateCornerMesh()
{
	auto CornerInstancedComponent = CreateInstancedStaticMeshComponent(
		"CornerInstance", StructureData.SecondaryStaticMesh,
		StructureData.SecondaryMaterialInstance);

	for (int i{}; i < SplineComponent->GetNumberOfSplinePoints(); ++i)
	{
		auto SplinePointTransform = SplineComponent->GetTransformAtSplinePoint(i, ESplineCoordinateSpace::Local, false);

		CornerInstancedComponent->AddInstance(SplinePointTransform);
	}
}

UInstancedStaticMeshComponent* ABGSplineStructure::CreateInstancedStaticMeshComponent(
	FString const& InstanceName, UStaticMesh* StaticMesh,
	UMaterialInstance* MaterialInstance)
{
	auto NewInstancedStaticMeshComponent = NewObject<UInstancedStaticMeshComponent>(this, *InstanceName);
	NewInstancedStaticMeshComponent->RegisterComponent();
	NewInstancedStaticMeshComponent->SetCollisionProfileName("Structure");
	NewInstancedStaticMeshComponent->SetIsReplicated(true);
	NewInstancedStaticMeshComponent->AttachToComponent(SplineComponent,
	                                                   FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewInstancedStaticMeshComponent->SetFlags(RF_Transactional);
	NewInstancedStaticMeshComponent->SetStaticMesh(StaticMesh);
	NewInstancedStaticMeshComponent->SetMaterial(0, MaterialInstance);
	this->AddInstanceComponent(NewInstancedStaticMeshComponent);

	return NewInstancedStaticMeshComponent;
}

void ABGSplineStructure::ToggleLockStructureInPlace_Implementation(bool const bLock)
{
	bLocked = bLock;
}

void ABGSplineStructure::AddPointToSpline_Implementation(FVector const& Location, float const& Key)
{
	FSplinePoint NewSplinePoint;
	NewSplinePoint.Type = ESplinePointType::Linear;
	NewSplinePoint.Position = Location;
	NewSplinePoint.ArriveTangent = FVector::ZeroVector;
	NewSplinePoint.LeaveTangent = FVector::ZeroVector;
	NewSplinePoint.Rotation = FRotator::ZeroRotator;
	NewSplinePoint.InputKey = Key;
	SplineComponent->AddPoint(NewSplinePoint);
	UpdateStructureMesh();
}

void ABGSplineStructure::SetStructureInfo(FBGStructureInfo const& NewStructureInfo)
{
	StructureData = NewStructureInfo;

	UpdateStructureMesh();
}

UInstancedStaticMeshComponent* ABGSplineStructure::GetInstancedStaticMeshComponentByString(
	FString const& InstanceName) const
{
	for (auto Instance : GetInstanceComponents())
	{
		if (Instance && Instance->GetName().Equals(InstanceName))
		{
			if (auto const CastInstance = Cast<UInstancedStaticMeshComponent>(Instance))
			{
				return CastInstance;
			}
		}
	}
	return nullptr;
}
