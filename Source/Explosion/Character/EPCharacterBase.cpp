// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEPCharacterBase::AEPCharacterBase()
{
	// 컨트롤러 회전
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 0.2f;

	// Mesh 설정
	GetMesh() ->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

// Called when the game starts or when spawned
void AEPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


