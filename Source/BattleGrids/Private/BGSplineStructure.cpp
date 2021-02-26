// © 2021 Matthew Barham. All Rights Reserved.


#include "BGSplineStructure.h"
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ABGSplineStructure::ABGSplineStructure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SplineComponent->SetupAttachment(RootComponent);

	// InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced Static Mesh"));
	// InstancedStaticMeshComponent->SetupAttachment(SplineComponent);
	// InstancedStaticMeshComponent->SetStaticMesh(StaticMeshReference);
	// InstancedStaticMeshComponent->SetMaterial(0, MaterialReference);
	// this->AddInstanceComponent(InstancedStaticMeshComponent);
}

void ABGSplineStructure::AddMeshToSpline()
{
	// currently not being used - Blueprint version is working
	// TODO: Fix this to match Blueprint functionality and move other SplineStructure Blueprint functions to C++
	
	if (StaticMeshReference && MaterialReference)
	{
		UInstancedStaticMeshComponent* NewInstancedMesh = NewObject<UInstancedStaticMeshComponent>(this);
		//NewInstancedMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		NewInstancedMesh->RegisterComponent();
		NewInstancedMesh->SetStaticMesh(StaticMeshReference);
		NewInstancedMesh->SetMaterial(0, MaterialReference);
		NewInstancedMesh->SetFlags(RF_Transactional);
		this->AddInstanceComponent(NewInstancedMesh);

		for (int i{}; i <= SplineComponent->GetNumberOfSplinePoints() - 1; ++i)
		{
			auto LocationOne = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			auto LocationTwo = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

			auto const Distance = FVector::Dist(LocationOne, LocationTwo);
			auto const RepeatWidth{105.f};

			int const Integral = FMath::TruncToInt(Distance / RepeatWidth);
			// int const Copies = 3;

			for (int j{}; j <= Integral; ++j)
			{
				FTransform NewTransform{};
				NewTransform.SetLocation(FVector(j * RepeatWidth, 0.f, 0.f));
				NewTransform.SetRotation(FQuat::Identity);
				NewTransform.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
				NewInstancedMesh->AddInstance(NewTransform);
			}
		}
	}

	// FTransform NewTransform;
	// NewTransform.SetLocation(SplineComponent->GetLocationAtSplinePoint(SplinePoint, ESplineCoordinateSpace::World));
	// NewTransform.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
	// InstancedStaticMeshComponent->AddInstance(NewTransform);

	// if (StaticMeshReference && MaterialReference)
	// {
	// 	UInstancedStaticMeshComponent* NewStaticMeshComponent = NewObject<UInstancedStaticMeshComponent>(this);
	// 	NewStaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	// 	NewStaticMeshComponent->RegisterComponent();
	// 	NewStaticMeshComponent->SetStaticMesh(StaticMeshReference);
	// 	NewStaticMeshComponent->SetMaterial(0, MaterialReference);
	// 	NewStaticMeshComponent->SetFlags(RF_Transactional);
	// 	this->AddInstanceComponent(NewStaticMeshComponent);
	//
	// }
}

// Called when the game starts or when spawned
void ABGSplineStructure::BeginPlay()
{
	Super::BeginPlay();
}

void ABGSplineStructure::OnConstruction(const FTransform& Transform)
{
	// AddMeshToSpline();
}
