// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UWRPG_ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "UWRPG_HUD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UUWRPG_HUD : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	URPG_AbilitySystemComponent* AbilitySystemComponent;
	
protected:
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUWRPG_ProgressBar* HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UUWRPG_ProgressBar* ManaBar;

	UFUNCTION(BlueprintCallable)
	void UpdateMaxHealth(float InMaxHealth);

	UFUNCTION(BlueprintCallable)
	void UpdateCurrentHealth(float InCurrentHealth);

	UFUNCTION(BlueprintCallable)
	void UpdateMaxMana(float InMaxMana);

	UFUNCTION(BlueprintCallable)
	void UpdateCurrentMana(float InCurrentMana);

	UFUNCTION()
	void SetRPGAbilitySystemComponent(URPG_AbilitySystemComponent* ASC) { AbilitySystemComponent = ASC; }

	UFUNCTION(BlueprintPure)
	URPG_AbilitySystemComponent* GetRPGAbilitySystemComponent() const { return AbilitySystemComponent; }
	
public:

	UFUNCTION()
	void InitializeHUD(URPG_AbilitySystemComponent* ASC);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "InitializeHUD")
	void K2_InitializeHUD(URPG_AbilitySystemComponent* ASC);
};
