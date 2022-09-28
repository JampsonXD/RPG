// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RPG_AttributeSet.h"

#include "GameplayEffectExtension.h"
#include "RPG_Character.h"
#include "GAS/RPG_AbilitySystemComponent.h"

URPG_AttributeSet::URPG_AttributeSet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void URPG_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void URPG_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);


	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		HandleDamageAttribute(Data);
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewHealth = FMath::Clamp(GetHealth(), 0.f, GetMaxHealth());
		SetHealth(NewHealth);
	}
	
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		const float NewMana = FMath::Clamp(GetMana(), 0.f, GetMaxMana());
		SetMana(NewMana);
	}

	if(Data.EvaluatedData.Attribute == GetExperienceAttribute())
	{
		const FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
		UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();

		HandleExperienceAttribute(Source);
	}
}

void URPG_AttributeSet::HandleDamageAttribute(const FGameplayEffectModCallbackData& Data)
{
	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	}

	const bool bImplementsRPGInterface = TargetActor->Implements<URPGCharacterInterface>();

	// Return early if we implement the rpg interface and are dead
	if (bImplementsRPGInterface && IRPGCharacterInterface::Execute_IsDead(TargetActor))
	{
		return;
	}

	const float DamageDone = GetDamage();
	SetDamage(0.f);

	if (DamageDone > 0.f)
	{
		const float NewHealth = FMath::Max<float>(GetHealth() - DamageDone, 0.f);
		SetHealth(NewHealth);

		if(NewHealth <= 0.f && bImplementsRPGInterface)
		{
			IRPGCharacterInterface::Execute_DeathEvent(TargetActor);
		}
	}
}

void URPG_AttributeSet::HandleExperienceAttribute(UAbilitySystemComponent* ASC)
{
	// Make sure our ASC is a valid RPG ASC before continuing
	URPG_AbilitySystemComponent* SourceRPGASC = Cast<URPG_AbilitySystemComponent>(ASC);
	if(!SourceRPGASC) return;
	
	float LevelsGained = 0.f;
	const float NewExperience = EvaluateExperienceGained(SourceRPGASC, GetExperience(), LevelsGained);
	if(LevelsGained > 0)
	{
		SetExperience(NewExperience);
		SourceRPGASC->GrantLevelUpAttributes(LevelsGained, GetCharacterLevel());
	}
}

float URPG_AttributeSet::EvaluateExperienceGained(URPG_AbilitySystemComponent* ASC, const float InExperience, float& LevelsGained)
{
	/* Iterates over this function until our Experience passed in is no longer higher than our needed
	 * experience for our next level. This way we can get the amount of levels we have gained and our
	 * experience leftover to reach our next character level
	 **/
	const float NeededExperience = ASC->GetExperienceForNextLevel();
	if(InExperience < NeededExperience)
	{
		return InExperience;
	}

	const float LeftoverExperience = InExperience - NeededExperience;
	ASC->SetNextLevelExperience();
	LevelsGained ++;
	
	return EvaluateExperienceGained(ASC, LeftoverExperience, LevelsGained);
}
