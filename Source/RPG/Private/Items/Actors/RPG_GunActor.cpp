// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Actors/RPG_GunActor.h"

#include "RPGBFL_MainFunctions.h"
#include "RPG_Character.h"
#include "RPG_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Items/RPG_SuperGun.h"

ARPG_GunActor::ARPG_GunActor() : Super()
{
	CurrentClipSize = 0;
	MaximumClipSize = 0;
}

void ARPG_GunActor::SetupItem_Implementation(UItem* ItemDataAsset)
{
	Super::SetupItem_Implementation(ItemDataAsset);

	ensure(ItemDataAsset);
	GunData = CastChecked<URPG_SuperGun>(ItemDataAsset);
	EffectsData = GunData->WeaponEffects;
	MaximumClipSize = GunData->ClipSize;
	CurrentClipSize = MaximumClipSize;
}

void ARPG_GunActor::EquipItem_Implementation(ARPG_Character* EquippingCharacter,
	URPG_AbilitySystemComponent* RPGAbilitySystemComponent)
{
	Super::EquipItem_Implementation(EquippingCharacter, RPGAbilitySystemComponent);

	EquippingCharacter->AttachItem(this, GunData->EquipData);

	if(GunData->CrosshairWidget)
	{
		if(ARPG_PlayerController* PC = Cast<ARPG_PlayerController>(EquippingCharacter->GetController()))
		{
			CrosshairWidget = CreateWidget(PC, GunData->CrosshairWidget);
			CrosshairWidget->AddToViewport(0);
		}
	}
}

void ARPG_GunActor::UnEquipItem_Implementation(ARPG_Character* EquippingCharacter,
	URPG_AbilitySystemComponent* RPGAbilitySystemComponent)
{
	Super::UnEquipItem_Implementation(EquippingCharacter, RPGAbilitySystemComponent);

	DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);

	if(CrosshairWidget)
	{
		CrosshairWidget->RemoveFromParent();
	}
}

bool ARPG_GunActor::CanFire_Implementation()
{
	return CurrentClipSize > 0;
}

bool ARPG_GunActor::CanReload_Implementation()
{
	return false;
}

void ARPG_GunActor::Reload_Implementation()
{

}

void ARPG_GunActor::FireWeapon_Implementation()
{
	if(!EffectsData)
	{
		return;
	}

	URPGBFL_MainFunctions::PlayEffectPackAttached(EffectsData->MuzzleEffects, this, nullptr,
		EEffectActivationType::Instant, GetWeaponMesh());
}
