// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGStructure.h"


#include "Actors/BGSplineStructure.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/BGContextMenu.h"

// Sets default values
ABGStructure::ABGStructure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	ContextMenuWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Context Menu Widget"));
	ContextMenuWidgetComponent->SetupAttachment(RootComponent);
	ContextMenuWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ContextMenuWidgetComponent->SetPivot(FVector2D(0.f, 0.f));
	ContextMenuWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	ContextMenuWidgetComponent->SetDrawAtDesiredSize(true);
	ContextMenuWidgetComponent->SetVisibility(false);

	bLocked = false;
}

// Called when the game starts or when spawned
void ABGStructure::BeginPlay()
{
	Super::BeginPlay();

	SetReplicatingMovement(true);

	// run this on the Server immediately, but wait for OnRep_ContextMenuClass() for Clients
	if (UKismetSystemLibrary::IsServer(this))
	{
		UpdateContextMenuWidget();
	}
}

// Called every frame
void ABGStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABGStructure::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGStructure, bLocked)
	DOREPLIFETIME(ABGStructure, ParentSplineStructure)
	DOREPLIFETIME(ABGStructure, IndexOnSplineStructure)
	DOREPLIFETIME(ABGStructure, ContextMenuClass)
}

void ABGStructure::OnRep_ContextMenuClass()
{
	UpdateContextMenuWidget();
}

void ABGStructure::UpdateContextMenuWidget()
{
	// If Client, Set the Widget Class
	if (!UKismetSystemLibrary::IsServer(this) && ContextMenuClass.GetDefaultObject())
	{
		ContextMenuWidgetComponent->SetWidgetClass(ContextMenuClass);
		ContextMenuWidgetComponent->InitWidget();
	}

	// Set Parent variable on the ContextMenu
	auto ContextMenu = GetContextMenu();
	if (ContextMenu)
	{
		ContextMenu->SetParent(this);
	}
}

void ABGStructure::SetParentSplineStructure(ABGSplineStructure* NewParentSplineStructure)
{
	ParentSplineStructure = NewParentSplineStructure;
}

void ABGStructure::SetWidgetComponentClass_Implementation(TSubclassOf<UUserWidget> InClass)
{
	ContextMenuClass.operator=(InClass);
	ContextMenuWidgetComponent->SetWidgetClass(InClass);
	ContextMenuWidgetComponent->InitWidget();
	UpdateContextMenuWidget();
}

void ABGStructure::ToggleContextMenu()
{
	if (ContextMenuWidgetComponent && ContextMenuWidgetComponent->GetWidget())
	{
		ContextMenuWidgetComponent->SetWorldLocation(GetContextMenu()->GetHitResult().ImpactPoint);
		ContextMenuWidgetComponent->ToggleVisibility();
	}
}

void ABGStructure::CloseContextMenu()
{
	if (ContextMenuWidgetComponent)
	{
		ContextMenuWidgetComponent->SetVisibility(false);
	}
}

UBGContextMenu* ABGStructure::GetContextMenu()
{
	return Cast<UBGContextMenu>(ContextMenuWidgetComponent->GetWidget());
}

void ABGStructure::ToggleStructureLock_Implementation(bool const bNewLocked)
{
	SetLocked(bNewLocked);
}
