// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPG_ItemTypes.generated.h"

/**
 * 
 */

/* Information needed for equipping a weapon or object, such as any offsets or rotations */
USTRUCT(BlueprintType)
struct FEquipData
{
	GENERATED_BODY()

	FEquipData()
	{
		AttachName = TEXT("root");
		RelativeTransform = FTransform::Identity;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform RelativeTransform;
	
};
