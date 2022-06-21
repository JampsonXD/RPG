// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/RPG_SuperWeapon.h"
#include "RPG_WeaponActor.generated.h"

struct FItemActorData;
UCLASS()
class RPG_API ARPG_WeaponActor : public AActor
{
	GENERATED_BODY()


	UPROPERTY()
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY()
	URPG_SuperWeapon* WeaponData;

	UPROPERTY()
	UInventorySystemComponent* InventorySystemComponent;

	UPROPERTY()
	AActor* OwningActor;

public:	
	// Sets default values for this actor's properties
	ARPG_WeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void InitializeWeapon();

	UFUNCTION(BlueprintPure)
	virtual USkeletalMeshComponent* GetWeaponMesh() const;

	UFUNCTION(BlueprintPure)
	virtual URPG_SuperWeapon* GetWeaponData() const;

	UFUNCTION(BlueprintPure)
	virtual UInventorySystemComponent* GetOwningInventorySystemComponent() const;

	UFUNCTION(BlueprintPure)
	virtual AActor* GetOwningActor() const;

};
