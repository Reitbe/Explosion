// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected: 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))	
	class ADefaultCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	bool IsMoving;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float MovementSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float ControllerYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivate = "true"))
	float ControllerPitch;

};
