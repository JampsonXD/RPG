// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_Character.h"

#include "RPGAssetManager.h"
#include "RPG_PlayerState.h"
#include "Components/CapsuleComponent.h"
#include "DataAssets/RPG_CharacterDataAsset.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "GAS/RPG_GameplayAbility.h"
#include "RPG/RPG.h"
#include "GAS/RPG_AttributeSet.h"

//////////////////////////////////////////////////////////////////////////
// ARPGCharacter

ARPG_Character::ARPG_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ChestArmorMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("ChestArmor"));
	ChestArmorMeshComponent->SetupAttachment(GetMesh());
	ChestArmorMeshComponent->SetMasterPoseComponent(GetMesh(), true);

	GauntletMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("GauntletArmor"));
	GauntletMeshComponent->SetupAttachment(GetMesh());
	GauntletMeshComponent->SetMasterPoseComponent(GetMesh(), true);

	HelmetMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("HelmetArmor"));
	HelmetMeshComponent->SetupAttachment(GetMesh());
	HelmetMeshComponent->SetMasterPoseComponent(GetMesh(), true);

	LegMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("LegArmor"));
	LegMeshComponent->SetupAttachment(GetMesh());
	LegMeshComponent->SetMasterPoseComponent(GetMesh(), true);

	// Add our armor map slots, these should have null values as we shouldn't have any armor equipped
	ArmorMap.Add(EArmorEquipSlot::Helmet, nullptr);
	ArmorMap.Add(EArmorEquipSlot::Chest, nullptr);
	ArmorMap.Add(EArmorEquipSlot::Gauntlets, nullptr);
	ArmorMap.Add(EArmorEquipSlot::Legs, nullptr);
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
}

UAbilitySystemComponent* ARPG_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

float ARPG_Character::GetCharacterLevel()
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet.Get()->GetCharacterLevel();
	}

	return 1.f;
}

bool ARPG_Character::IsEquipped(UItem* Item) const
{
	if(!Item)
	{
		return false;
	}
	
	// Loop through our armor and check if we have the item equipped
	for(const TPair<EArmorEquipSlot, UArmorItem*> Pair : ArmorMap)
	{
		if(Pair.Value == Item)
		{
			return true;
		}
	}

	return false;
}

bool ARPG_Character::TryEquipArmor(UArmorItem* Armor)
{
	if(!Armor)
	{
		return false;
	}

	if(UArmorItem** PreviousArmor = ArmorMap.Find(Armor->GetEquipSlot()))
	{
		// If our previous armor is the same as our new armor, stop the process here
		if(*PreviousArmor == Armor)
		{
			return false;
		}
		UnequipArmor(*PreviousArmor);
	}
	
	USkeletalMeshComponent* MeshComponent = GetMeshComponentFromArmorSlotType(Armor->GetEquipSlot());

	if(!MeshComponent)
	{
		return false;
	}
	
	MeshComponent->SetSkeletalMesh(Armor->GetArmorMesh());
	Armor->AddAbilitySet(GetAbilitySystemComponent());
	ArmorMap.Add(Armor->GetEquipSlot(), Armor);
	return true;
}

void ARPG_Character::UnequipArmor(UArmorItem* ArmorItem)
{
	if(!ArmorItem)
	{
		return;
	}

	// Remove our granted ability set and set the armor map back to null
	ArmorItem->RemoveAbilitySet();
	ArmorMap.Add(ArmorItem->GetEquipSlot(), nullptr);

	// Try getting our mesh component for our armor and remove the mesh
	if(USkeletalMeshComponent* MeshComponent = GetMeshComponentFromArmorSlotType(ArmorItem->GetEquipSlot()))
	{
		MeshComponent->SetSkeletalMesh(nullptr);
	}
}

void ARPG_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ARPG_PlayerState* PS = GetPlayerState<ARPG_PlayerState>();
	if(PS)
	{
		// Set our ASC
		AbilitySystemComponent = Cast<URPG_AbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Set Actor Info - Owner is Player State, Avatar is our Character
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set our Attribute Pointers
		InitAttributeSets(PS);

		// Activate our default Ability Set if its not currently active
		if(!DefaultAbilitySetHandle.IsActive())
		{
			DefaultAbilitySetHandle = AbilitySystemComponent->AddAbilitySet(DefaultAbilitySet, this);
		}
	}
}

USkeletalMeshComponent* ARPG_Character::GetMeshComponentFromArmorSlotType(EArmorEquipSlot EquipSlot) const
{
	switch (EquipSlot)
	{
		case EArmorEquipSlot::Chest :
			return ChestArmorMeshComponent;

		case EArmorEquipSlot::Gauntlets :
			return GauntletMeshComponent;

		case EArmorEquipSlot::Helmet :
			return HelmetMeshComponent;

		case EArmorEquipSlot::Legs :
			return LegMeshComponent;

		default:
			return nullptr;
	}
}

void ARPG_Character::InitAttributeSets(class ARPG_PlayerState* PS)
{
	AttributeSet = PS->GetAttributeSet();
}

void ARPG_Character::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	URPGAssetManager& AssetManager = URPGAssetManager::Get();
	TArray<FName> Bundles = TArray<FName>();
	Bundles.Add(FName("InWorld"));
	const FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ARPG_Character::OnCharacterDataLoaded, CharacterDataAssetID);
	AssetManager.LoadPrimaryAsset(CharacterDataAssetID, Bundles, Delegate, FStreamableManager::AsyncLoadHighPriority);
}

void ARPG_Character::OnCharacterDataLoaded(FPrimaryAssetId LoadedId)
{
	const URPGAssetManager& AssetManager = URPGAssetManager::Get();

	if(const URPG_CharacterDataAsset* CharacterData = Cast<URPG_CharacterDataAsset>(AssetManager.GetPrimaryAssetObject(LoadedId)))
	{
		GetMesh()->SetSkeletalMesh(CharacterData->CharacterMesh);
		GetMesh()->SetAnimInstanceClass(CharacterData->AnimClass);
	}
}
