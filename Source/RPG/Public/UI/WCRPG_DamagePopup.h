// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG_Types.h"
#include "Components/WidgetComponent.h"
#include "WCRPG_DamagePopup.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UWCRPG_DamagePopup : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	UWCRPG_DamagePopup(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage Popup")
	void SetupDamagePopup(FDamagePopupData data);
};
