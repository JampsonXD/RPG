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

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void URPG_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	ARPG_Character* TargetCharacter = nullptr;
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<ARPG_Character>(TargetActor);
	}

	// Get the source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	ARPG_Character* SourceCharacter = nullptr;
	if(Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->PlayerController.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		
		if(SourceController == nullptr && SourceActor != nullptr)
		{
			if(APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if(SourceController)
		{
			SourceCharacter = Cast<ARPG_Character>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<ARPG_Character>(SourceActor);
		}

		if(Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageDone = GetDamage();
		SetDamage(0.f);

		if(DamageDone > 0.f)
		{
			const float NewHealth = FMath::Max<float>(GetHealth() - DamageDone, 0.f);
			SetHealth(NewHealth);
		}
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
		HandleExperienceAttribute(Source);
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
