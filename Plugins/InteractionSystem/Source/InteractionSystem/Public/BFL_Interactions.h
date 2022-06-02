// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InteractTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_Interactions.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionDelegate, FInteractInfo, InteractInfo);
/**
 * 
 */
UCLASS()
class INTERACTIONSYSTEM_API UBFL_Interactions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
};
