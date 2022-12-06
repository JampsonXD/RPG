// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AbilityTasks/AT_FireGun.h"

#include "AbilitySystemComponent.h"
#include "Items/RPG_SuperGun.h"


UAT_FireGun::UAT_FireGun()
{
	FireMode = EGunFireMode::SemiAutomatic;
	RateOfFire = 1;
	TotalShotsFired = 0;
}

UAT_FireGun* UAT_FireGun::FireWeapon(UGameplayAbility* OwningAbility, URPG_SuperGun* GunDef, FGameplayTag CooldownTag, bool bCommitAbility)
{
	UAT_FireGun* Object = NewAbilityTask<UAT_FireGun>(OwningAbility);
	check(GunDef)
	Object->bShouldCommitAbility = bCommitAbility;
	Object->TotalShotsFired = 0;
	Object->FireMode = GunDef->FireMode;
	Object->RateOfFire = GunDef->FireRate;

	return Object;
}

void UAT_FireGun::HandleWeaponFire()
{
	if(bShouldCommitAbility && !CanFireWeapon())
	{
		if(ShouldBroadcastAbilityTaskDelegates())
		{
			CommitAbilityFailed.Broadcast();
		}

		EndTask();
	}

	++TotalShotsFired;
	if(ShouldBroadcastAbilityTaskDelegates())
	{
		WeaponFired.Broadcast();
	}

	switch (FireMode)
	{
		case EGunFireMode::SemiAutomatic:
			CommitAbilityCooldown();
			FinishedFiring.Broadcast();
			EndTask();

		case EGunFireMode::BurstFire:
		case EGunFireMode::Automatic:
			SetWeaponFireTimer();
			break;
	}
}

void UAT_FireGun::SetupAutomaticWeaponInput()
{
	// Bind to our input and make sure we haven't already released the button
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		if (IsLocallyControlled())
		{
			FGameplayAbilitySpec *Spec = Ability->GetCurrentAbilitySpec();
			if (Spec && !Spec->InputPressed)
			{
				OnReleaseCallback();
			}
		}

		DelegateHandle = ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased,
		                                                     GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UAT_FireGun::OnReleaseCallback);
		if (IsForRemoteClient())
		{
			if (!ASC->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()))
			{
				SetWaitingOnRemotePlayerData();
			}
		}
	}
}

void UAT_FireGun::Activate()
{
	if(!Ability)
	{
		return;
	}

	
	HandleWeaponFire();

	if(FireMode == EGunFireMode::Automatic)
	{
		SetupAutomaticWeaponInput();
	}
	
}

void UAT_FireGun::OnDestroy(bool bInOwnerFinished)
{
	UWorld* World = GetWorld();
	if(World)
	{
		World->GetTimerManager().ClearAllTimersForObject(this);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

bool UAT_FireGun::CanFireWeapon() const
{
	if(Ability->IsInstantiated())
	{
		// Only commit our ability cost as we will handle commit the ability cooldown if needed, automatic and burst weapons handle weapon fire through timers
		return Ability->CommitAbilityCost(GetAbilitySpecHandle(), Ability->GetCurrentActorInfo(), Ability->GetCurrentActivationInfo());
	}

	return false;
}

void UAT_FireGun::SetWeaponFireTimer()
{
	/* TODO : Set the burst fire information in the gun definition instead of hard coding a value */
	if(FireMode == EGunFireMode::BurstFire && TotalShotsFired >= 3.f)
	{
		CommitAbilityCooldown();
		FinishedFiring.Broadcast();
		EndTask();
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimer(WeaponFireTimerHandle, this, &UAT_FireGun::HandleWeaponFire, 60.f / RateOfFire, false);
}

void UAT_FireGun::OnReleaseCallback()
{
	UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
	if (!Ability || !ASC)
	{
		return;
	}

	ASC->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);

	FScopedPredictionWindow ScopedPrediction(ASC, IsPredictingClient());

	if (IsPredictingClient())
	{
		// Tell the server about this
		ASC->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey(), ASC->ScopedPredictionKey);
	}
	else
	{
		ASC->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	// We are done. Kill us so we don't keep getting broadcast messages
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		InputReleased.Broadcast();
	}
	
	CommitAbilityCooldown();
	EndTask();
}

void UAT_FireGun::CommitAbilityCooldown()
{
	if(Ability)
	{
		Ability->CommitAbilityCooldown(GetAbilitySpecHandle(), Ability->GetCurrentActorInfo(), Ability->GetCurrentActivationInfo(), true);
	}
}

