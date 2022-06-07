// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "UWRPG_ProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UUWRPG_ProgressBar : public UUserWidget
{
	GENERATED_BODY()

	protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UOverlay* Overlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ProgressText;

	UPROPERTY(BlueprintReadOnly)
	float CurrentValue;
	
	UPROPERTY(BlueprintReadOnly)
	float MaxValue;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FGameplayAttribute GameplayAttribute;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly)
	FGameplayAttribute GameplayAttributeMax;

	UFUNCTION()
	void UpdateProgress() const;

	UFUNCTION()
	float GetProgressBarPercentage() const;

public:

	UUWRPG_ProgressBar(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION()
	void UpdateProgressUsingAttribute(const URPG_AbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable)
	void SetProgressCurrentValue(float& InCurrentValue);

	UFUNCTION(BlueprintCallable)
	void SetProgressMaxValue(float& InMaxValue);
	
};
