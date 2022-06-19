// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Actors/RPG_WeaponActor.h"
#include "RPG_Types.h"

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

void ARPG_WeaponActor::InitializeWeapon(const FItemActorData& ItemData)
{
	WeaponData = ItemData.WeaponObject;
	InventorySystemComponent = ItemData.InventorySystemComponent;
	OwningActor = ItemData.OwningActor;

	if(WeaponData->GetWeaponMesh())
	{
		WeaponMesh->SetSkeletalMesh(WeaponData->GetWeaponMesh());
	}
}

USkeletalMeshComponent* ARPG_WeaponActor::GetWeaponMesh() const
{
	return WeaponMesh;
}

URPG_SuperWeapon* ARPG_WeaponActor::GetWeaponData() const
{
	return WeaponData;
}

UInventorySystemComponent* ARPG_WeaponActor::GetOwningInventorySystemComponent() const
{
	return InventorySystemComponent;
}

AActor* ARPG_WeaponActor::GetOwningActor() const
{
	return OwningActor;
}

