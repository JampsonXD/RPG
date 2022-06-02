// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AT_WaitTime.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWaitFinished);

UCLASS(BlueprintType, meta=(ExposedAsyncProxy = AsyncTask))
class RPG_API UAT_WaitTime : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UObject* WorldContextObject;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintAssignable)
	FOnWaitFinished WaitFinished;

	UFUNCTION()
	void TimerFinished(bool bCallOnce);
	
public:

	UFUNCTION(BlueprintCallable, Category = "Blueprint Task | Wait", meta = (BlueprintInternalUseOnly = "true", WorldContext = "InWorldContextObject"))
	static UAT_WaitTime* WaitForTimePassed(UObject* InWorldContextObject, float TimeToWait, bool bCallOnce = true);
	
	UFUNCTION(BlueprintCallable, Category = "Blueprint Task | Wait")
	void EndTask();
	
};
