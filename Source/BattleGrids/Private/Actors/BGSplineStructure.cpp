// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGSplineStructure.h"
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ABGSplineStructure::ABGSplineStructure()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(RootComponent);
	SplineComponent->SetIsReplicated(true);

	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	InstancedStaticMeshComponent->SetupAttachment(SplineComponent);
	this->AddInstanceComponent(InstancedStaticMeshComponent);
}

void ABGSplineStructure::SetStructurePhysicsAndCollision_Implementation(bool const bGravityOn,
                                                                        ECollisionEnabled::Type const CollisionType)
{
	InstancedStaticMeshComponent->SetEnableGravity(bGravityOn);
	InstancedStaticMeshComponent->SetCollisionEnabled(CollisionType);
}

void ABGSplineStructure::AddMeshToSpline_Implementation()
{
	if (StaticMeshReference && MaterialReference)
	{
		// clear all instances (to handle shrinking spline)
		InstancedStaticMeshComponent->ClearInstances();
		int LoopCount{};

		for (int i{}; i <= SplineComponent->GetNumberOfSplinePoints() - 2; ++i)
		{
			auto LocationOne = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			auto LocationTwo = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

			auto const Distance = FVector::Dist(LocationOne, LocationTwo);
			auto const RepeatWidth{105.f};

			int const Integral = FMath::TruncToInt(Distance / 100.f);

			for (int j{}; j <= Integral; ++j)
			{
				FTransform NewTransform{};

				auto SplineLocation = SplineComponent->GetLocationAtDistanceAlongSpline(
					LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);
				auto SplineRotation = SplineComponent->GetRotationAtDistanceAlongSpline(
					LoopCount * RepeatWidth, ESplineCoordinateSpace::Local);

				NewTransform.SetLocation(SplineLocation);
				NewTransform.SetRotation(FQuat(SplineRotation));
				NewTransform.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
				InstancedStaticMeshComponent->AddInstance(NewTransform);

				++LoopCount;
			}
		}
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

// Called when the game starts or when spawned
void ABGSplineStructure::BeginPlay()
{
	Super::BeginPlay();

	SetReplicatingMovement(true);
}

void ABGSplineStructure::OnConstruction(const FTransform& Transform)
{
	if (StaticMeshReference && MaterialReference)
	{
		InstancedStaticMeshComponent->SetStaticMesh(StaticMeshReference);
		InstancedStaticMeshComponent->SetMaterial(0, MaterialReference);
		AddMeshToSpline();
	}
}
