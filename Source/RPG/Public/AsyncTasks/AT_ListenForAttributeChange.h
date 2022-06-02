// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GAS/RPG_AttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AT_ListenForAttributeChange.generated.h"

// Multicast delegate used for broadcasting attribute changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameplayAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

/**
 * 
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy = AsyncTask))
class RPG_API UAT_ListenForAttributeChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	FGameplayAttribute GameplayAttribute;

	UPROPERTY()
	TArray<FGameplayAttribute> GameplayAttributes;
	
	void OnAttributeChanged(const FOnAttributeChangeData& Data) const;
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnGameplayAttributeChanged GameplayAttributeChanged;

	UFUNCTION(BlueprintCallable, Category = "ListenForAttributeChanged", meta = (BlueprintInternalUseOnly = "true"))
	static UAT_ListenForAttributeChange* ListenForAttributeChange(UAbilitySystemComponent* InAbilitySystemComponent, FGameplayAttribute InAttribute);

	UFUNCTION(BlueprintCallable, Category = "ListenForAttributeChanged", meta = (BlueprintInternalUseOnly = "true"))
	static UAT_ListenForAttributeChange* ListenForAttributesChanged(UAbilitySystemComponent* InAbilitySystemComponent, TArray<FGameplayAttribute> InAttributes);

	UFUNCTION(BlueprintCallable, Category = "ListenForAttributeChanged")
	void EndTask();
	
};
