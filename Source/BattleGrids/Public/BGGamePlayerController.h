// © 2021 Matthew Barham. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BGPlayerController.h"
#include "BGTypes.h"

#include "BGGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEGRIDS_API ABGGamePlayerController : public ABGPlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "BGGamePlayerController|Movement")
	void UpdateTransformOnServer(FTransform NewTransform);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, Category = "BGGamePlayerController|TokenTransform")
	void RotateToken(float Value);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "BGGamePlayerController|Config")
	EBGControlMode ControlMode {EBGControlMode::Move};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGamePlayerController|Config")
	class ABGToken* GrabbedToken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGamePlayerController|Config")
	class ABGSplineStructure* GrabbedStructure;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGGamePlayerController|Config")
	class ABGBoard* GrabbedBoard;
};
