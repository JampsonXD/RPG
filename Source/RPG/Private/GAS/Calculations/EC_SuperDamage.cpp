// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Calculations/EC_SuperDamage.h"

#include "RPG_GameSingleton.h"
#include "RPG_PlayerController.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "GAS/RPG_AttributeSet.h"

UEC_SuperDamage::UEC_SuperDamage()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicArmorDef);
}

void UEC_SuperDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	URPG_AbilitySystemComponent* SourceASC = Cast<URPG_AbilitySystemComponent>(ExecutionParams.GetSourceAbilitySystemComponent());
	URPG_AbilitySystemComponent* TargetASC = Cast<URPG_AbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());

	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer& PassedInTags = ExecutionParams.GetPassedInTags();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;
	
	// Find a matching Damage Type from our singleton and call its calculation to get our return damage
	const URPG_GameSingleton* Singleton = URPG_GameSingleton::GetSingleton();
	const FDamageTypeData DamageTypeData = Singleton->GetDamageTypeDataFromContainer(PassedInTags);
	
	// Evaluate our Damage Attribute using both our attribute magnitude and any passed in set by caller magnitude values
	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluateParameters, Damage);
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f),0.0f);

	// If our Damage Mitigation Mod Calc is valid, grab its CDO and get our modified damage value
	float PostMitigationDamage = Damage;
	if(DamageTypeData.DamageMitigationMod.Get())
	{
		URPG_DamageMitigationCalculations* CDODamageCalc = DamageTypeData.DamageMitigationMod.GetDefaultObject();
		PostMitigationDamage = CDODamageCalc->GetModifiedValue(Damage, ExecutionParams, EvaluateParameters, DamageStatics());
	}

	if(PostMitigationDamage <= 0.f) return;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, PostMitigationDamage));

	ARPG_PlayerController* SourceController = SourceActor ? Cast<ARPG_PlayerController>(SourceActor->GetInstigatorController()) : nullptr;
	const FHitResult* HitResult = Spec.GetContext().GetHitResult();

	if(SourceController && TargetActor)
	{
		const FVector DamageLocation = HitResult ? HitResult->ImpactPoint : TargetActor->GetActorLocation();
		const FDamagePopupData PopupData = FDamagePopupData(DamageTypeData.DamageColor, Damage, false, DamageLocation);
		SourceController->AddPendingDamagePopup(PopupData);
	}
	
}

bool UEC_SuperDamage::DoesRequirePassedInTags() const
{
	return true;
}
