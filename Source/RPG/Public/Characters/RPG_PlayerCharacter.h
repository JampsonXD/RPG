// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InteractableInterface.h"
#include "InteractionSystemInterface.h"
#include "RPG_Character.h"
#include "InputMappingContext.h"
#include "RPG_PlayerCharacter.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class RPG_API ARPG_PlayerCharacter : public ARPG_Character, public IInteractionSystemInterface, public IInteractableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class UInteractionSystemComponent* InteractionSystemComponent;

	// Flag for if our ASC Input is already bound
	bool bASCInputBound = false;

public:

	ARPG_PlayerCharacter(const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputMappingContext* MainMappingContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Actions")
	UInputAction* MovementInputAction;

	virtual void PawnClientRestart() override;
	
	void OnPlayerMovementInput(const FInputActionValue& ActionValue);
	
	virtual FInteractionTracePoints GetTracePoints() const override;

	virtual bool PreInteract_Implementation(FInteractInfo InteractInfo) override;
	virtual void Interact_Implementation(FInteractInfo InteractInfo) override;
	virtual void PostInteract_Implementation(FInteractInfo InteractInfo, EInteractionEndState EndState) override;

	UFUNCTION()
	void BindASCInput();

	/** 
	* Called via input to turn at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate. 
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns our Interaction System Component **/
	virtual UInteractionSystemComponent* GetInteractionSystemComponent() const override;
	
};
