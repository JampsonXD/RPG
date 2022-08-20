// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/EquippableItemInterface.h"
#include "Items/RPG_SuperWeapon.h"
#include "RPG_WeaponActor.generated.h"

struct FItemActorData;
UCLASS()
class RPG_API ARPG_WeaponActor : public AActor, public IEquippableItemInterface
{
	GENERATED_BODY()


	UPROPERTY()
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY()
	UInventorySystemComponent* InventorySystemComponent;

	UPROPERTY()
	AActor* OwningActor;

	UPROPERTY()
	UAbilitySet* AbilitySet;

	UPROPERTY()
	FAbilitySetActiveHandle AbilitySetActiveHandle;

public:	
	// Sets default values for this actor's properties
	ARPG_WeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupItem_Implementation(UItem* ItemDataAsset) override;

	virtual void EquipItem_Implementation(ARPG_Character* EquippingCharacter, URPG_AbilitySystemComponent* RPGAbilitySystemComponent) override;

	virtual void UnEquipItem_Implementation(ARPG_Character* EquippingCharacter, URPG_AbilitySystemComponent* RPGAbilitySystemComponent) override;

	UFUNCTION(BlueprintPure)
	virtual USkeletalMeshComponent* GetWeaponMesh() const;

	UFUNCTION(BlueprintPure)
	virtual UInventorySystemComponent* GetOwningInventorySystemComponent() const;

	UFUNCTION(BlueprintPure)
	virtual AActor* GetOwningActor() const;

};
