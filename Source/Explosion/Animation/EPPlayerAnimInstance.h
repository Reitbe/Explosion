// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EPPlayerAnimInstance.generated.h"

class AEPCharacterPlayer;
class UCharacterMovementComponent;

UCLASS()
class EXPLOSION_API UEPPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEPPlayerAnimInstance();


protected: 
// 캐릭터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))	
	TObjectPtr<AEPCharacterPlayer> EPCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

// 점프
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	float JumpingThreshold;

// 이동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	uint8 bIsMoving : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	FVector MovingVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	double Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	float MovementThreshold;

// Aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	float ControllerPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	uint8 bIsBombAiming : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivate = "true"))
	uint8 bIsBombThrowing : 1;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

};
