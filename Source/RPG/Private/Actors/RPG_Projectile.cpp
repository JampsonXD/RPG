// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RPG_Projectile.h"

#include "RPGBFL_MainFunctions.h"

class UPoolWorldSubsystem;
// Sets default values
ARPG_Projectile::ARPG_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	CollisionComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void ARPG_Projectile::OnPoolEntered_Implementation()
{
	// Clear our life span timer, stop simulating our projectile movement
	SetLifeSpan(0);
	const FHitResult HitResult;
	ProjectileMovementComponent->StopSimulating(HitResult);


}

void ARPG_Projectile::OnPoolLeft_Implementation(const FActorPopData& PopData)
{
	// Set our projectile lifespan and set data for our projectile movement
	SetLifeSpan(PopData.Magnitude);
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->Velocity = PopData.Velocity;
}

void ARPG_Projectile::LifeSpanExpired()
{
	K2_LifeSpanExpired();
}

void ARPG_Projectile::ReturnProjectileToPool()
{
	URPGBFL_MainFunctions::AddActorToPool(this);
}


