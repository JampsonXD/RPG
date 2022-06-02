// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UQuestDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	FName QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	FText QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Data")
	float Experience;
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
};
