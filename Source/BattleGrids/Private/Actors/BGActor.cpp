// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGActor.h"

#include "Characters/BGToken.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UI/BGContextMenu.h"

// Sets default values
ABGActor::ABGActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	RootComponent = RootSceneComponent;

	ContextMenuWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Context Menu Widget"));
	ContextMenuWidgetComponent->SetupAttachment(RootComponent);
	ContextMenuWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	ContextMenuWidgetComponent->SetPivot(FVector2D(0.f, 0.f));
	ContextMenuWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	ContextMenuWidgetComponent->SetDrawAtDesiredSize(true);
	ContextMenuWidgetComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void ABGActor::BeginPlay()
{
	Super::BeginPlay();

	SetReplicatingMovement(true);

	// run this on the Server immediately, but wait for OnRep_ContextMenuClass() for Clients
	if (UKismetSystemLibrary::IsServer(this))
	{
		UpdateContextMenuWidget();
	}
}

void ABGActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGActor, ContextMenuClass)
}

void ABGActor::OnRep_ContextMenuClass()
{
	UpdateContextMenuWidget();
}

void ABGActor::UpdateContextMenuWidget()
{
	// if Client, set the Widget Class
	if (!UKismetSystemLibrary::IsServer(this) && ContextMenuClass.GetDefaultObject())
	{
		ContextMenuWidgetComponent->SetWidgetClass(ContextMenuClass);
		ContextMenuWidgetComponent->InitWidget();
	}

	// Set Parent variable if not already set on the ContextMenu
	auto ContextMenu = GetContextMenu();
	if (ContextMenu)
	{
		ContextMenu->SetActorParent(this);
	}
}

void ABGActor::SetWidgetComponentClass_Implementation(TSubclassOf<UUserWidget> InClass)
{
	ContextMenuClass.operator=(InClass);
	ContextMenuWidgetComponent->SetWidgetClass(InClass);
	ContextMenuWidgetComponent->InitWidget();
	UpdateContextMenuWidget();
}

void ABGActor::ToggleContextMenu()
{
	if (ContextMenuWidgetComponent && ContextMenuWidgetComponent->GetWidget())
	{
		ContextMenuWidgetComponent->SetWorldLocation(GetContextMenu()->GetHitResult().ImpactPoint);
		ContextMenuWidgetComponent->ToggleVisibility();
	}
}

void ABGActor::CloseContextMenu()
{
	if (ContextMenuWidgetComponent)
	{
		ContextMenuWidgetComponent->SetVisibility(false);
	}
}

UBGContextMenu* ABGActor::GetContextMenu()
{
	return Cast<UBGContextMenu>(ContextMenuWidgetComponent->GetWidget());
}
