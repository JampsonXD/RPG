// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_Character.h"

#include "RPGAssetManager.h"
#include "RPG_PlayerState.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/RPG_CharacterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameplayTag/RPG_TagLibrary.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "GAS/RPG_GameplayAbility.h"
#include "RPG/RPG.h"
#include "GAS/RPG_AttributeSet.h"

//////////////////////////////////////////////////////////////////////////
// ARPGCharacter

ARPG_Character::ARPG_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

UAbilitySystemComponent* ARPG_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

float ARPG_Character::GetCharacterLevel()
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet.Get()->GetCharacterLevel();
	}

	return 1.f;
}

void ARPG_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ARPG_PlayerState* PS = GetPlayerState<ARPG_PlayerState>();
	if(PS)
	{
		// Set our ASC
		AbilitySystemComponent = Cast<URPG_AbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Set Actor Info - Owner is Player State, Avatar is our Character
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set our Attribute Pointers
		InitAttributeSets(PS);

		// Activate our default Ability Set if its not currently active
		if(!DefaultAbilitySetHandle.IsActive() && DefaultAbilitySet)
		{
			DefaultAbilitySetHandle = AbilitySystemComponent->AddAbilitySet(DefaultAbilitySet, this);
		}
	}
}



void ARPG_Character::InitAttributeSets(class ARPG_PlayerState* PS)
{
	AttributeSet = PS->GetAttributeSet();
}

void ARPG_Character::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	URPGAssetManager& AssetManager = URPGAssetManager::Get();
	TArray<FName> Bundles = TArray<FName>();
	Bundles.Add(FName("InWorld"));
	const FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ARPG_Character::OnCharacterDataLoaded, CharacterDataAssetID);
	AssetManager.LoadPrimaryAsset(CharacterDataAssetID, Bundles, Delegate, FStreamableManager::AsyncLoadHighPriority);
}

void ARPG_Character::OnCharacterDataLoaded(FPrimaryAssetId LoadedId)
{
	const URPGAssetManager& AssetManager = URPGAssetManager::Get();

	if(const URPG_CharacterDataAsset* CharacterData = Cast<URPG_CharacterDataAsset>(AssetManager.GetPrimaryAssetObject(LoadedId)))
	{
		GetMesh()->SetSkeletalMesh(CharacterData->CharacterMesh);
		GetMesh()->SetAnimInstanceClass(CharacterData->AnimClass);
	}
}

void ARPG_Character::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent.Get()->GetOwnedGameplayTags(TagContainer);
	}
}

bool ARPG_Character::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (AbilitySystemComponent.IsValid())
	{
		return AbilitySystemComponent.Get()->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool ARPG_Character::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent.IsValid())
	{
		return AbilitySystemComponent.Get()->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool ARPG_Character::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (AbilitySystemComponent.IsValid())
	{
		return AbilitySystemComponent.Get()->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool ARPG_Character::IsAiming_Implementation() const
{
	return HasMatchingGameplayTag(FRPG_TagLibrary::Get().AimingTag());
}

bool ARPG_Character::IsRunning_Implementation() const
{
	return HasMatchingGameplayTag(FRPG_TagLibrary::Get().RunningTag());
}

bool ARPG_Character::IsCrouching_Implementation() const
{
	return HasMatchingGameplayTag(FRPG_TagLibrary::Get().CrouchingTag());
}

bool ARPG_Character::IsWeaponLowered_Implementation() const
{
	return HasMatchingGameplayTag(FRPG_TagLibrary::Get().WeaponLoweredTag());
}

bool ARPG_Character::IsJumping_Implementation() const
{
	return HasMatchingGameplayTag(FRPG_TagLibrary::Get().JumpingTag());
}
