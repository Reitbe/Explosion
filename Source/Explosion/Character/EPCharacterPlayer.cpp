// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosion/Character/EPCharacterPlayer.h"
#include "Explosion/Animation/EPPlayerAnimInstance.h"
#include "Explosion/Bomb/EPBombBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"


AEPCharacterPlayer::AEPCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 설정
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;


	// 에셋 로드 - 입력
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder
	(TEXT("/Game/Input/IMC_Character.IMC_Character"));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActoinMoveFinder
	(TEXT("/Game/Input/IA_Moving.IA_Moving"));
	if (InputActoinMoveFinder.Succeeded())
	{
		Moving = InputActoinMoveFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActoinLookFinder
	(TEXT("/Game/Input/IA_Looking.IA_Looking"));
	if (InputActoinLookFinder.Succeeded())
	{
		Looking = InputActoinLookFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpFinder
	(TEXT("/Game/Input/IA_Jump.IA_Jump"));
	if (InputActionJumpFinder.Succeeded())
	{
		Jumping = InputActionJumpFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAimingBombFinder
	(TEXT("/Game/Input/IA_AimingBomb.IA_AimingBomb"));
	if (InputActionAimingBombFinder.Succeeded())
	{
		AimingBomb = InputActionAimingBombFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThrowingBombFinder
	(TEXT("/Game/Input/IA_ThrowingBomb.IA_ThrowingBomb"));
	if (InputActionThrowingBombFinder.Succeeded())
	{
		ThrowingBomb = InputActionThrowingBombFinder.Object;
	}

	// 에셋 로드 - 애니메이션
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationBlueprintFinder
	(TEXT("/Game/Animation/AnimationBlueprint/ABP_Player.ABP_Player_C"));
	if (AnimationBlueprintFinder.Class)
	{
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(AnimationBlueprintFinder.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowingMontageFinder
	(TEXT("/Game/Animation/Montage/AM_Throwing.AM_Throwing"));
	if (ThrowingMontageFinder.Succeeded())
	{
		ThrowingMontage = ThrowingMontageFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AimingMontageFinder
	(TEXT("/Game/Animation/Montage/AM_Aiming.AM_Aiming"));
	if (AimingMontageFinder.Succeeded())
	{
		AimingMontage = AimingMontageFinder.Object;
	}

	// 폭탄 설정
	DamageMultiplier = 1.0f;
	ThrowingDistance = 10000.0f;
}

void AEPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr) {
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	AnimInstance = GetMesh()->GetAnimInstance();

	// 폭탄을 던지고 재장전 할 때의 대리자 연결(순서주의)
	OnThrowingBombDelegate.AddUObject(this, &AEPCharacterPlayer::OnThrowingBomb);
	OnReloadingBomb();
	OnReloadingBombDelegate.BindUObject(this, &AEPCharacterPlayer::OnReloadingBomb);
}

void AEPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEPCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(Moving, ETriggerEvent::Triggered, this, &AEPCharacterPlayer::Move);
		Input->BindAction(Looking, ETriggerEvent::Triggered, this, &AEPCharacterPlayer::Look);
		Input->BindAction(Jumping, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		Input->BindAction(AimingBomb, ETriggerEvent::Started, this, &AEPCharacterPlayer::AimingOn);
		Input->BindAction(AimingBomb, ETriggerEvent::Completed, this, &AEPCharacterPlayer::AimingOff);
		Input->BindAction(ThrowingBomb, ETriggerEvent::Started, this, &AEPCharacterPlayer::Throwing);
	}
}

void AEPCharacterPlayer::Move(const FInputActionValue& Value)
{
	if (PlayerController)
	{
		// 크기
		float FowardValue = Value.Get<FVector2D>().Y;
		float SideValue = Value.Get<FVector2D>().X;

		// 방향
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector SideDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, FowardValue);
		AddMovementInput(SideDirection, SideValue);
	}
}

void AEPCharacterPlayer::Look(const FInputActionValue& Value)
{
	if (PlayerController)
	{
		AddControllerYawInput(Value.Get<FVector2D>().X);
		AddControllerPitchInput(Value.Get<FVector2D>().Y);
	}
}

void AEPCharacterPlayer::AimingOn()
{
	if (bIsThrowing)
	{
		return;
	}
	bIsAiming = true;

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(AimingMontage);
	}

	// 조준 시 차징 발동
	GetWorldTimerManager().SetTimer(ChargingRateTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			DamageMultiplier = 2.0f;
		}
	), 2.0f, false);
}

void AEPCharacterPlayer::AimingOff()
{
	GetWorldTimerManager().ClearTimer(ChargingRateTimerHandle);

	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.3f, AimingMontage);
	}

	DamageMultiplier = 1.0f;
	bIsAiming = false;
}

void AEPCharacterPlayer::Throwing()
{
	if (bIsThrowing) 
	{
		return;
	}
	bIsThrowing = true;

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ThrowingMontage);

		// 폭탄 던지기가 끝났을 때의 대리자 연결
		FOnMontageBlendingOutStarted BlendingOutDelegate;
		BlendingOutDelegate.BindUObject(this, &AEPCharacterPlayer::Throwing_OnMontageEnded);
		AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, ThrowingMontage);
	}

	// 미조준 상태 - 타이머 작동X
	if (bIsAiming == false) 
	{	
		DamageMultiplier = 1.0f;
	}
	// 조준 상태 - 타이머 작동O
	else
	{
		// 풀충전
		if ((DamageMultiplier == 2.0f))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("조준O - 최대 충전으로 던지기"));
		}
		// 비례 충전
		else 
		{
			DamageMultiplier += GetWorldTimerManager().GetTimerElapsed(ChargingRateTimerHandle) / 2.0f;
			//FString str = FString::Printf(TEXT("조준O - 비례 충전으로 던지기 - 가중치 : %f"), DamageMultiplier);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, str);
		}
	}
}

void AEPCharacterPlayer::Throwing_OnMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsThrowing = false;
	if (bIsAiming)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AimingMontage);
		}

		DamageMultiplier = 1.0f;
		// 조준 중이었다면 차징 재발동
		GetWorldTimerManager().ClearTimer(ChargingRateTimerHandle);
		GetWorldTimerManager().SetTimer(ChargingRateTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				DamageMultiplier = 2.0f;
			}
		), 2.0f, false);
	}
}

void AEPCharacterPlayer::OnReloadingBomb()
{
	BombInstance = GetWorld()->SpawnActor<AEPBombBase>(BP_Bomb, FVector::ZeroVector, FRotator::ZeroRotator);
	if (BombInstance)
	{
		OnThrowingBombDelegate.AddUObject(BombInstance, &AEPBombBase::OnThrowingBomb);
		FName BombSocket(TEXT("BombHolder"));
		BombInstance->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			BombSocket
		);
	}
}

void AEPCharacterPlayer::OnThrowingBomb()
{
	if (BombInstance)
	{
		// 바라보는 방향
		FRotator Rotation = GetControlRotation();
		FVector Direction = Rotation.Vector();

		// 던지기(임시)
		BombInstance->GetBombMeshComponent()->AddImpulse(Direction * DamageMultiplier * ThrowingDistance);
		
		OnThrowingBombDelegate.RemoveAll(BombInstance);
		
		BombInstance->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		BombInstance = nullptr;
	}
}




