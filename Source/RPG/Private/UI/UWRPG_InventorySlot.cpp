// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UWRPG_InventorySlot.h"

#include "IDetailTreeNode.h"
#include "ItemTypes.h"


void UUWRPG_InventorySlot::BeginDestroy()
{
	Super::BeginDestroy();
}

void UUWRPG_InventorySlot::SetupInventorySlot(FInventorySlot& InInventorySlot, UInventorySystemComponent* ISC)
{
	SetInventorySlot(InInventorySlot);
	SetInventorySystemComponent(ISC);
	K2_SetupInventorySlot(InInventorySlot);
}

FInventorySlot UUWRPG_InventorySlot::GetInventorySlot() const
{
	return InventorySlot;
}

UInventorySystemComponent* UUWRPG_InventorySlot::GetInventorySystemComponent() const
{
	return InventorySystemComponent;
}

void UUWRPG_InventorySlot::SetInventorySlot(const FInventorySlot& InInventorySlot)
{
	InventorySlot = InInventorySlot;
}

void UUWRPG_InventorySlot::SetInventorySystemComponent(UInventorySystemComponent* ISC)
{
	InventorySystemComponent = ISC;
}

void UUWRPG_InventorySlot::StackCountChanged(int& NewStackCount)
{
	K2_StackCountChanged(NewStackCount);
}
