// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "DefaultCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ADefaultCharacter>(TryGetPawnOwner());

}


void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character) 
	{
		IsInAir = Character->GetCharacterMovement()->IsFalling();

		FVector velocity = Character->GetVelocity();
		velocity = FVector(velocity.X, velocity.Y, 0.0f);

		MovementSpeed = FVector(velocity.X, velocity.Y, 0.0f).Size();
		MovementDirection = CalculateDirection(velocity, Character->GetActorRotation());
	}
}
