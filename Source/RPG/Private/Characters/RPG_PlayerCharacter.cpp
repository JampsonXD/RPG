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

ARPG_PlayerCharacter::ARPG_PlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractionSystemComponent = CreateDefaultSubobject<UInteractionSystemComponent>(TEXT("Interaction System Component"));
}

FInteractionTracePoints ARPG_PlayerCharacter::GetTracePoints() const
{
	FInteractionTracePoints TracePoints;

	FRotator eyeRotation;
	GetActorEyesViewPoint(TracePoints.StartLocation, eyeRotation);
	TracePoints.ForwardVector = UKismetMathLibrary::GetForwardVector(eyeRotation);

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
	const FVector MovementVector = ActionValue.Get<FVector>();
	const float X = MovementVector.X;
	const float Y = MovementVector.Y;

	if(Y != 0)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Y);
	}

	if(X != 0)
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

void ARPG_PlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_PlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPG_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	if(UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerEnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &ARPG_PlayerCharacter::OnPlayerMovementInput);
	}
	
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPG_PlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPG_PlayerCharacter::LookUpAtRate);

	// Bind our Ability System Component Input
	BindASCInput();
}

UInteractionSystemComponent* ARPG_PlayerCharacter::GetInteractionSystemComponent() const
{
	return InteractionSystemComponent;
}
