// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerAnimInstance.h"
#include "Explosion/Character/EPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UEPPlayerAnimInstance::UEPPlayerAnimInstance()
{
	MovementThreshold = 20.0f;
	JumpingThreshold = 100.0f;
}

void UEPPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<AEPCharacterPlayer>(GetOwningActor());
	if (Character)
	{
		CharacterMovementComponent = Character->GetCharacterMovement();
	}
}


void UEPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (CharacterMovementComponent) 
	{
		// 캐릭터 평면 움직임
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();
		FTransform CharacterTransform = Character->GetActorTransform();
		FVector CharacterLocalXDirection = CharacterTransform.InverseTransformVectorNoScale(Velocity);
		Angle = CharacterLocalXDirection.ToOrientationRotator().Yaw;

		bIsMoving = GroundSpeed > MovementThreshold;

		// Jump
		bIsFalling = CharacterMovementComponent->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);

		// Aim
		FRotator Delta = Character->GetControlRotation() - Character->GetActorRotation();
		Delta.Normalize();
		ControllerPitch = Delta.Pitch;
	}
}
