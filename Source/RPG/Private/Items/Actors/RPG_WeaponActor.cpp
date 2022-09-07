// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Actors/RPG_WeaponActor.h"

#include "RPG_Character.h"
#include "RPG_Types.h"
#include "GAS/RPG_AbilitySystemComponent.h"

// Sets default values
ARPG_WeaponActor::ARPG_WeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
}

// Called when the game starts or when spawned
void ARPG_WeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPG_WeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARPG_WeaponActor::SetupItem_Implementation(FItemSetupData ItemSetupData)
{
	const URPG_SuperWeapon* WeaponData = CastChecked<URPG_SuperWeapon>(ItemSetupData.DataAsset);
	OwningActor = ItemSetupData.OwningActor;
	InventorySystemComponent = ItemSetupData.OwningInventorySystemComponent;

	WeaponMesh->SetSkeletalMesh(WeaponData->Mesh);
	AbilitySet = WeaponData->ItemAbilitySet;
}

void ARPG_WeaponActor::EquipItem_Implementation(ARPG_Character* EquippingCharacter,
	URPG_AbilitySystemComponent* RPGAbilitySystemComponent)
{
	if(AbilitySet)
	{
		AbilitySetActiveHandle = AbilitySet->AddAbilitySet(RPGAbilitySystemComponent, this);
	}
}

void ARPG_WeaponActor::UnEquipItem_Implementation(ARPG_Character* EquippingCharacter,
	URPG_AbilitySystemComponent* RPGAbilitySystemComponent)
{
	AbilitySetActiveHandle.Remove();
}

USkeletalMeshComponent* ARPG_WeaponActor::GetWeaponMesh() const
{
	return WeaponMesh;
}

UInventorySystemComponent* ARPG_WeaponActor::GetOwningInventorySystemComponent() const
{
	return InventorySystemComponent;
}

AActor* ARPG_WeaponActor::GetOwningActor() const
{
	return OwningActor;
}

