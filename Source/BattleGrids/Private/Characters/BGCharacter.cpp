// © 2021 Matthew Barham. All Rights Reserved.


#include "Characters/BGCharacter.h"

// Sets default values
ABGCharacter::ABGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

