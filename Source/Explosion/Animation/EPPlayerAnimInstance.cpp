// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerAnimInstance.h"
#include "Explosion/Character/EPCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UEPPlayerAnimInstance::UEPPlayerAnimInstance()
{
	// 임계값 이하의 움직임은 정지 상태로 간주
	MovementThreshold = 20.0f;
	JumpingThreshold = 100.0f;

	MovingVelocity = FVector::ZeroVector;
	GroundSpeed = 0.0f;
	Angle = 0.0f;
	ControllerPitch = 0.0f;
}

void UEPPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EPCharacter = Cast<AEPCharacterPlayer>(GetOwningActor());
	if (EPCharacter)
	{
		CharacterMovementComponent = EPCharacter->GetCharacterMovement();
	}
}


void UEPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (CharacterMovementComponent) 
	{
		// 캐릭터의 평면 이동 속도
		MovingVelocity = CharacterMovementComponent->Velocity;
		GroundSpeed = MovingVelocity.Size2D();

		// 캐릭터의 정면 기준 이동 방향
		FTransform CharacterTransform = EPCharacter->GetActorTransform();
		FVector CharacterLocalXDirection = CharacterTransform.InverseTransformVectorNoScale(MovingVelocity);
		Angle = CharacterLocalXDirection.ToOrientationRotator().Yaw;

		// 임계 이하 움직임은 정지 상태로 간주
		bIsMoving = GroundSpeed > MovementThreshold;

		// 점프
		bIsFalling = CharacterMovementComponent->IsFalling();
		bIsJumping = bIsFalling & (MovingVelocity.Z > JumpingThreshold);

		// 에임 오프셋용 컨트롤러 피치
		FRotator Delta = EPCharacter->GetControlRotation() - EPCharacter->GetActorRotation();
		Delta.Normalize();
		ControllerPitch = Delta.Pitch;
	}
}

