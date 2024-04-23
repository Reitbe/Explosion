// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EPPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEPPlayerAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected: 
// Character
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))	
	TObjectPtr<class AEPCharacterPlayer> Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	TObjectPtr<class UCharacterMovementComponent> CharacterMovement;

// Jumping
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float JumpingThreshold;

// Moving
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	uint8 bIsMoving : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float MovementThreshold;

// Aiming
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float ControllerYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float ControllerPitch;

};
