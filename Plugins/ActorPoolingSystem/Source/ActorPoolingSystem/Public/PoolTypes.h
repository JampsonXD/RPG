// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "PoolTypes.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FDefaultActorPoolData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinimumPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaximumPoolSize;
};

USTRUCT(BlueprintType)
struct FActorPopData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	AActor* Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	APawn* Instigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	const UObject* OptionalObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	const UObject* OptionalObject2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	FGameplayTagContainer OptionalGameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	float Magnitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool Pop Data")
	float OptionalMagnitude;

	virtual APawn* GetInstigator() const
	{
		return Instigator;
	}

	virtual AActor* GetOwner() const
	{
		return Owner;
	}

	virtual FVector GetLocation() const
	{
		return Location;
	}

	virtual FRotator GetRotator() const
	{
		return Rotation;
	}
};

USTRUCT(BlueprintType)
struct FActorPool
{
	GENERATED_BODY()

	FActorPool()
	{
		MinimumPoolSize = 1;
		MaximumPoolSize = 10;
	}

	FActorPool(int InMinimumPoolSize, int InMaximumPoolSize)
	{
		MinimumPoolSize = InMinimumPoolSize;
		MaximumPoolSize = InMaximumPoolSize;
	}


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool")
	TArray<AActor*> Pool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool")
	int MinimumPoolSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Pool")
	int MaximumPoolSize;

	bool ShouldGrow() const;

	bool CanGrow() const;

	bool CanShrink() const;

	void Push(AActor* Actor);

	AActor* Pop();

	int Num() const;

	bool ContainsActor(AActor* Actor) const;

};
