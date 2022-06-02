// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "InteractionSystemComponent.h"
#include "InteractTypes.generated.h"

/**
 * 
 */

// Used with our end interaction state, whether the interaction ended successfully or not
UENUM()
enum class EInteractionEndState : uint8
{
	None,
	Cancelled,
	EndSuccessful,
	Interrupted
};

USTRUCT(BlueprintType)
struct FInteractInfo
{
   GENERATED_BODY()

	FInteractInfo()
   {
   		InteractingActor = nullptr;
   		InteractingInteractionSystemComponent = nullptr;
   		InteractingController = nullptr;
   }
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AActor* InteractingActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UInteractionSystemComponent* InteractingInteractionSystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AController* InteractingController;
	
	static FInteractInfo CreateInteractInfo(UInteractionSystemComponent* InteractingISC)
	{
		FInteractInfo InteractInfo;
		InteractInfo.InteractingInteractionSystemComponent = InteractingISC;
		InteractInfo.InteractingActor =  InteractingISC->GetOwner();
		InteractInfo.InteractingController = InteractInfo.InteractingActor->GetInstigatorController();

		return InteractInfo;
	}
};
