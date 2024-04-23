// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerAnimInstance.h"
#include "Explosion/Character/EPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UEPPlayerAnimInstance::UEPPlayerAnimInstance()
{
	MovementThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UEPPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AEPCharacterPlayer>(GetOwningActor());
	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}


void UEPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (CharacterMovement) 
	{
		// Movement
		Velocity = CharacterMovement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsMoving = GroundSpeed > MovementThreshold;

		// Jump
		bIsFalling = CharacterMovement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);

		// Idle
		bIsIdle = GroundSpeed < MovementThreshold;
		
		// Aim
		ControllerYaw = Character->GetControlRotation().Yaw;
		ControllerPitch = Character->GetControlRotation().Pitch;
	}
}
