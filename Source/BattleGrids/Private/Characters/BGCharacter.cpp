// © 2021 Matthew Barham. All Rights Reserved.

#include "Characters/BGCharacter.h"

#include "Net/UnrealNetwork.h"
#include "UI/BGContextMenu.h"

ABGCharacter::ABGCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABGCharacter::Destroyed()
{
	Super::Destroyed();

	FacadeActor->Destroy();
}

void ABGCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABGCharacter::OnRep_FacadeActor()
{
	SetFacadeActorContextMenuParent();
}

void ABGCharacter::SetFacadeActorContextMenuParent_Implementation()
{
	auto ContextMenu = FacadeActor->GetContextMenu();
	if (ContextMenu)
	{
		ContextMenu->SetActorParent(this);
	}
}

void ABGCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABGCharacter, FacadeActor)
}

void ABGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABGCharacter::SetWidgetComponentClass_Implementation(TSubclassOf<UUserWidget> InClass)
{
	/** Create the Facade Actor then set its WidgetComponentClass */
	auto const World = GetWorld();
	if (World)
	{
		FacadeActor = World->SpawnActorDeferred<ABGActor>(
			ABGActor::StaticClass(), FTransform::Identity, this);
		if (!ensure(FacadeActor)) return;

		FacadeActor->SetWidgetComponentClass(InClass);
		
		FacadeActor->FinishSpawning(FTransform::Identity);

		/** 
		* Redirect the ContextMenu's parent from the FacadeActor to this Character
		* Server does it immediately, clients will wait for a replicated function
		*/
		auto ContextMenu = FacadeActor->GetContextMenu();
		if (ContextMenu)
		{
			ContextMenu->SetActorParent(this);
		}
	}
}

void ABGCharacter::ToggleContextMenu()
{
	if (!ensure(FacadeActor)) return;
	FacadeActor->ToggleContextMenu();
}

void ABGCharacter::CloseContextMenu()
{
	if (!ensure(FacadeActor)) return;
	FacadeActor->CloseContextMenu();
}

UBGContextMenu* ABGCharacter::GetContextMenu()
{
	if (!ensure(FacadeActor)) return nullptr;
	return FacadeActor->GetContextMenu();
}
