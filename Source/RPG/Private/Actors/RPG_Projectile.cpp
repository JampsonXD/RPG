// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPG_Projectile.h"

// Sets default values
ARPG_Projectile::ARPG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	CollisionComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void ARPG_Projectile::OnPoolEntered()
{
	// Clear our life span timer, stop simulating our projectile movement
	SetLifeSpan(0);
	const FHitResult HitResult;
	ProjectileMovementComponent->StopSimulating(HitResult);

}

void ARPG_Projectile::OnPoolLeft(const FActorPopData& PopupData)
{
	// Set our projectile lifespan and set data for our projectile movement
	SetLifeSpan(PopupData.Magnitude);
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->Velocity = PopupData.Velocity;
}

void ARPG_Projectile::LifeSpanExpired()
{
	K2_LifeSpanExpired();
}


