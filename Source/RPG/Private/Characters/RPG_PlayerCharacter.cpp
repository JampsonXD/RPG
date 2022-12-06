// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPG_PlayerCharacter.h"
#include "InteractionSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/GAS_Types.h"
#include "GAS/RPG_AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "Items/Actors/RPG_WeaponActor.h"

ARPG_PlayerCharacter::ARPG_PlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;


	// Mesh Setup
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.f));
	GetMesh()->SetRelativeRotation(FRotator(0, 0, -90.f));

	// Camera Setup
	FirstPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FP Spring Arm"));
	FirstPersonSpringArm->SetupAttachment(GetCapsuleComponent());
	FirstPersonSpringArm->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	FirstPersonSpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	FirstPersonSpringArm->bUsePawnControlRotation = true;
	FirstPersonSpringArm->bEnableCameraLag = true;
	FirstPersonSpringArm->bEnableCameraRotationLag = true;
	FirstPersonSpringArm->CameraLagSpeed = 10.f;
	FirstPersonSpringArm->CameraRotationLagSpeed = 50.f;
	FirstPersonSpringArm->CameraLagMaxDistance = 500.f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP Camera"));
	FirstPersonCamera->SetupAttachment(FirstPersonSpringArm);
	FirstPersonCamera->SetFieldOfView(97.f);

	ThirdPersonSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TP Spring Arm"));
	ThirdPersonSpringArm->SetupAttachment(GetMesh(), "Head");
	ThirdPersonSpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TP Camera"));
	ThirdPersonCamera->SetupAttachment(ThirdPersonSpringArm);


	/* Weapon System */
	CurrentWeaponEquipSlot = -1;
	

				/* Interaction System Component */
	/****************************************************/
	/****************************************************/
	InteractionSystemComponent = CreateDefaultSubobject<UInteractionSystemComponent>(TEXT("Interaction System Component"));
}

void ARPG_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Try equipping our first weapon if one was added during possession
	EquipNextWeapon();
}

FInteractionTracePoints ARPG_PlayerCharacter::GetTracePoints() const
{
	FInteractionTracePoints TracePoints;

	FRotator EyeRotation;
	GetActorEyesViewPoint(TracePoints.StartLocation, EyeRotation);
	TracePoints.ForwardVector = UKismetMathLibrary::GetForwardVector(EyeRotation);

	return TracePoints;
}

bool ARPG_PlayerCharacter::PreInteract_Implementation(FInteractInfo InteractInfo)
{
	return IInteractableInterface::PreInteract_Implementation(InteractInfo);
}

void ARPG_PlayerCharacter::Interact_Implementation(FInteractInfo InteractInfo)
{
	IInteractableInterface::Interact_Implementation(InteractInfo);
}

void ARPG_PlayerCharacter::PostInteract_Implementation(FInteractInfo InteractInfo, EInteractionEndState EndState)
{
	IInteractableInterface::PostInteract_Implementation(InteractInfo, EndState);
}

void ARPG_PlayerCharacter::BindASCInput()
{
	if(!bASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		// Bind our ASC Input
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("Confirm"),
			FString("Cancel"),
			FString("EAbilityInput"),
			static_cast<int32>(EAbilityInput::Confirm),
			static_cast<int32>(EAbilityInput::Cancel)));

		bASCInputBound = true;
	}
}

void ARPG_PlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	// Make sure we have a valid player controller
	if(const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the enhanced input local player subsystem related to our player controller
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// Clear out any leftover mappings
			Subsystem->ClearAllMappings();

			// Add our mapping context
			Subsystem->AddMappingContext(MainMappingContext, 0);
		}
	}
}

void ARPG_PlayerCharacter::OnPlayerMovementInput(const FInputActionValue& ActionValue)
{
	if(FMath::IsNearlyZero(ActionValue.GetMagnitude()))
	{
		return;
	}

	const float X = ActionValue[0];
	const float Y = ActionValue[1];

	if(Y != 0.0f)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Y);
	}

	if(X != 0.0f)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, X);
	}
}

void ARPG_PlayerCharacter::OnPlayerCameraInput(const FInputActionValue& ActionValue)
{
	const FVector MovementVector = ActionValue.Get<FVector>();
	const float X = MovementVector.X;
	const float Y = MovementVector.Y;

	if(X != 0)
	{
		AddControllerYawInput(X * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}

	if(Y != 0)
	{
		AddControllerPitchInput(Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void ARPG_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up game play key bindings
	check(PlayerInputComponent);

	if(UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerEnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ARPG_PlayerCharacter::OnPlayerMovementInput);
		PlayerEnhancedInputComponent->BindAction(CameraInputAction, ETriggerEvent::Triggered, this, &ARPG_PlayerCharacter::OnPlayerCameraInput);
		PlayerEnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Triggered, this, &ARPG_PlayerCharacter::EquipNextWeapon);
		PlayerEnhancedInputComponent->BindAction(PreviousWeaponAction, ETriggerEvent::Triggered, this, &ARPG_PlayerCharacter::EquipPreviousWeapon);
	}
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind our Ability System Component Input
	BindASCInput();
}

UInteractionSystemComponent* ARPG_PlayerCharacter::GetInteractionSystemComponent() const
{
	return InteractionSystemComponent;
}

void ARPG_PlayerCharacter::EquipWeaponAtSlot(int EquipSlot)
{
	if(!IsValidNewWeaponEquipSlot(EquipSlot))
	{
		return;
	}

	UnequipCurrentWeapon();

	CurrentWeaponEquipSlot = EquipSlot;
	IEquippableItemInterface::Execute_EquipItem(WeaponActors[EquipSlot], this, GetRPGAbilitySystemComponent());
}

void ARPG_PlayerCharacter::EquipNextWeapon()
{
	const int NextSlot = CurrentWeaponEquipSlot + 1;
	EquipWeaponAtSlot(NextSlot < WeaponActors.Num() ? NextSlot : 0);
}

void ARPG_PlayerCharacter::EquipPreviousWeapon()
{
	const int PreviousSlot = CurrentWeaponEquipSlot - 1;
	EquipWeaponAtSlot(PreviousSlot > -1 ? PreviousSlot : WeaponActors.Num() - 1);
}

bool ARPG_PlayerCharacter::HasWeaponEquipped() const
{
	return CurrentWeaponEquipSlot > -1;
}

bool ARPG_PlayerCharacter::IsValidNewWeaponEquipSlot(const int EquipSlot) const
{
	if (CurrentWeaponEquipSlot == EquipSlot) return false;
	return IsValidWeaponSlot(EquipSlot);
}

bool ARPG_PlayerCharacter::IsValidWeaponSlot(const int NewWeaponSlot) const
{
	return NewWeaponSlot > -1 && NewWeaponSlot < WeaponActors.Num();
}

void ARPG_PlayerCharacter::UnequipCurrentWeapon()
{
	if(!HasWeaponEquipped())
	{
		return;
	}

	IEquippableItemInterface::Execute_UnEquipItem(WeaponActors[CurrentWeaponEquipSlot], this, GetRPGAbilitySystemComponent());
	CurrentWeaponEquipSlot = -1;
}
