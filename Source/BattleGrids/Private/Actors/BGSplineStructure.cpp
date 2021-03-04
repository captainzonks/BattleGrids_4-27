// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGSplineStructure.h"
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

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
}

void ABGSplineStructure::CreateDoorAtIndex_Implementation(int const& Index)
{
	if (SecondaryStructureDataArray[0].StaticMesh && SecondaryStructureDataArray[0].MaterialInstance)
	{
		FTransform SavedTransform;
		GetWallInstancedStaticMeshComponent()->GetInstanceTransform(Index, SavedTransform, false);
		GetWallInstancedStaticMeshComponent()->RemoveInstance(Index);

		auto NewInstancedStaticMeshComponent = NewObject<UInstancedStaticMeshComponent>(this, TEXT("DoorInstance"));
		NewInstancedStaticMeshComponent->RegisterComponent();
		NewInstancedStaticMeshComponent->SetIsReplicated(true);
		NewInstancedStaticMeshComponent->SetWorldLocation(RootComponent->GetComponentLocation());
		NewInstancedStaticMeshComponent->AttachToComponent(RootComponent,
                                                           FAttachmentTransformRules::SnapToTargetIncludingScale);
		NewInstancedStaticMeshComponent->SetFlags(RF_Transactional);
		NewInstancedStaticMeshComponent->SetStaticMesh(SecondaryStructureDataArray[0].StaticMesh);
		NewInstancedStaticMeshComponent->SetMaterial(0, SecondaryStructureDataArray[0].MaterialInstance);
		this->AddInstanceComponent(NewInstancedStaticMeshComponent);
		NewInstancedStaticMeshComponent->AddInstance(SavedTransform);
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

void ABGSplineStructure::AddMeshToSpline_Implementation()
{
	CleanupInstanceComponents();
	for (auto StructureInfo : PrimaryStructureDataArray)
	{
		if (StructureInfo.StaticMesh && StructureInfo.MaterialInstance)
		{
			auto NewInstancedStaticMeshComponent = NewObject<UInstancedStaticMeshComponent>(this, "WallInstance");
			NewInstancedStaticMeshComponent->RegisterComponent();
			NewInstancedStaticMeshComponent->SetIsReplicated(true);
			NewInstancedStaticMeshComponent->SetWorldLocation(RootComponent->GetComponentLocation());
			NewInstancedStaticMeshComponent->AttachToComponent(RootComponent,
			                                                   FAttachmentTransformRules::SnapToTargetIncludingScale);
			NewInstancedStaticMeshComponent->SetFlags(RF_Transactional);
			NewInstancedStaticMeshComponent->SetStaticMesh(StructureInfo.StaticMesh);
			NewInstancedStaticMeshComponent->SetMaterial(0, StructureInfo.MaterialInstance);
			this->AddInstanceComponent(NewInstancedStaticMeshComponent);

			int LoopCount{};

			for (int i{}; i <= SplineComponent->GetNumberOfSplinePoints() - 2; ++i)
			{
				auto LocationOne = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
				auto LocationTwo = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

				auto const Distance = FVector::Dist(LocationOne, LocationTwo);
				// don't hardcode repeat-width (eventually remove and replace with globally set variable)
				auto const RepeatWidth{105.f};

				int const Integral = FMath::TruncToInt(Distance / 100.f);

				for (int j{}; j <= Integral; ++j)
				{
					FTransform NewTransform{};

					const auto SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
						LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);
					auto SplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(
						LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);

					FVector NewLocation{};
					NewLocation.X = SplineLocation.X + StructureInfo.Transform.GetLocation().X;
					NewLocation.Y = SplineLocation.Y + StructureInfo.Transform.GetLocation().Y;
					NewLocation.Z = SplineLocation.Z + StructureInfo.Transform.GetLocation().Z;

					FRotator NewRotation{};
					NewRotation.Pitch = SplineRotation.Pitch + StructureInfo.Transform.GetRotation().Rotator().Pitch;
					NewRotation.Yaw = SplineRotation.Yaw + StructureInfo.Transform.GetRotation().Rotator().Yaw;
					NewRotation.Roll = SplineRotation.Roll + StructureInfo.Transform.GetRotation().Rotator().Roll;

					NewTransform.SetLocation(NewLocation);
					NewTransform.SetRotation(FQuat(NewRotation));
					NewTransform.SetScale3D(StructureInfo.Transform.GetScale3D());
					NewInstancedStaticMeshComponent->AddInstance(NewTransform);

					++LoopCount;
				}
			}
		}

		SetStructurePhysicsAndCollision(true, ECollisionEnabled::QueryAndPhysics);
	}
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

UInstancedStaticMeshComponent* ABGSplineStructure::GetWallInstancedStaticMeshComponent() const
{
	if (GetInstanceComponents().Num() > 0)
	{
		for (auto Instance : GetInstanceComponents())
		{
			if (Instance)
			{
				if (Instance->GetName().Equals(FString("WallInstance")))
				{
					if (auto const InstancedCast = Cast<UInstancedStaticMeshComponent>(Instance))
					{
						return InstancedCast;
					}
				}
			}
		}
	}
	return nullptr;
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

	AddMeshToSpline();
}

void ABGSplineStructure::CleanupInstanceComponents() const
{
	auto CurrentInstanceComponents = this->GetInstanceComponents();

	if (CurrentInstanceComponents.Num() > 0)
	{
		for (auto Instance : CurrentInstanceComponents)
		{
			if (Instance->IsValidLowLevel() && !Instance->IsPendingKillOrUnreachable())
				Instance->DestroyComponent();
		}
	}
}
