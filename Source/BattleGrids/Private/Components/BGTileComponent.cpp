// © 2021 Matthew Barham. All Rights Reserved.


#include "Components/BGTileComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UBGTileComponent::UBGTileComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bEditableWhenInherited = true;
}

void UBGTileComponent::ToggleTileVisibility_Implementation(bool bHide)
{
	StaticMeshComponent->SetVisibility(!bHide, true);
	StaticMeshComponent->SetCollisionEnabled(bHide
		                                         ? ECollisionEnabled::Type::QueryOnly
		                                         : ECollisionEnabled::Type::QueryAndPhysics);
	StaticMeshComponent->SetCanEverAffectNavigation(bHide ? false : true);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, bHide ? ECR_Ignore : ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, bHide ? ECR_Ignore : ECR_Block);
}

// Called when spawned
void UBGTileComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass(),
	                                                      TEXT("Tile Static Mesh"));
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetupAttachment(GetOwner()->GetRootComponent());
		StaticMeshComponent->RegisterComponent();
		StaticMeshComponent->CreationMethod = EComponentCreationMethod::Instance;
		StaticMeshComponent->SetCollisionProfileName("Tile");
		StaticMeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
		StaticMeshComponent->SetIsReplicated(true);
	}
}

void UBGTileComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBGTileComponent, BoardActorReference)
	DOREPLIFETIME(UBGTileComponent, TileInfo)
}
