// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "RPG_PrimaryAttackInterface.generated.h"

USTRUCT(BlueprintType)
struct FAttackMontageContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> Montages;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URPG_PrimaryAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IRPG_PrimaryAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Contract for getting a map of gameplay tags mapped to animation montages
	virtual TMap<FGameplayTag, FAttackMontageContainer> GetPrimaryAttackMap() = 0;
};
