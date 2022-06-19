// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/RPG_SuperGun.h"

int URPG_SuperGun::GetMaxClipSize() const
{
	return ClipSize;
}

float URPG_SuperGun::GetFireRate() const
{
	return FireRate;
}

EGunFireMode URPG_SuperGun::GetFireMode() const
{
	return FireMode;
}
