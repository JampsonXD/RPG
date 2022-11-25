// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LifetimeChecker.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FOnLifetimeCompleted);
UCLASS(Blueprintable)
class RPG_API ULifetimeChecker : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<AActor> WatchingActor;
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsTickable;
	
protected:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Initialize"))
	void K2_Initialize(AActor* Actor);

	UFUNCTION()
	virtual void Initialize(AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Deinitialize"))
	void K2_Deinitialize();

	UFUNCTION()
	virtual void Deinitialize();

	UFUNCTION(BlueprintCallable)
	virtual void LifetimeCompleted();
	
public:

	ULifetimeChecker(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void StartChecker(AActor* Actor);

	UFUNCTION()
	void StopChecker();

	/* Delegate bound that is invoked when the lifetime criteria are met */ 
	FOnLifetimeCompleted OnLifetimeCompleted;
	
	/* Native implementable tick event */ 
	UFUNCTION()
	virtual void Tick(float DeltaTime);

	/* Blueprint implementable tick event */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void K2_Tick(float DeltaTime);

	UFUNCTION()
	bool IsTickable() const
	{
		return bIsTickable;
	}
};
