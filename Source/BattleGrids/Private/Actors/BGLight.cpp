// © 2021 Matthew Barham. All Rights Reserved.


#include "Actors/BGLight.h"
#include "UI/BGContextMenu.h"

void ABGLight::BeginPlay()
{
	Super::BeginPlay();

	if (LightContextMenuClass.GetDefaultObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("Light Context Menu Class is valid"))

		SetWidgetComponentClass(LightContextMenuClass);
	}
}