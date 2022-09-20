// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledActorInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RPG_Projectile.generated.h"

UCLASS(BlueprintType, Blueprintable)
class RPG_API ARPG_Projectile : public AActor, public IPooledActorInterface
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CollisionComponent;


public:	
	// Sets default values for this actor's properties
	ARPG_Projectile();

	// Start of Pooled Actor Interface overrides
	virtual void OnPoolEntered() override;

	virtual void OnPoolLeft(const FActorPopData& PopupData) override;

	// End of Pooled Actor Interface overrides

	// Called when an actors lifespan ends, overridden to work with the actor pooling system
	virtual void LifeSpanExpired() override;


protected:

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "LifeSpanExpired"))
	void K2_LifeSpanExpired();

	UFUNCTION(BlueprintPure)
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovementComponent; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE UCapsuleComponent* GetCapsuleCollisionComponent() const { return CollisionComponent; }

};
