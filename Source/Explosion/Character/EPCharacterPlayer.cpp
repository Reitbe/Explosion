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

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;


	// 에셋 로드
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

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationBlueprintFinder
	(TEXT("/Game/Animation/AnimationBlueprint/ABP_Player.ABP_Player_C"));
	if (AnimationBlueprintFinder.Class)
	{
		// 애니메이션 세팅
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(AnimationBlueprintFinder.Class);
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

	// 폭탄 세팅
	DamageMultiplier = 1.0f;
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

	// 폭탄 대리자 관련
	OnThrowingBombDelegate.AddUObject(this, &AEPCharacterPlayer::OnThrowingBomb);

	OnReloadingBomb();
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
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(AimingMontage);
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("조준"));
	GetWorldTimerManager().SetTimer(ChargingRateTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			DamageMultiplier = 2.0f;
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("초기 2초 타이머"));
		}
	), 2.0f, false);
}

void AEPCharacterPlayer::AimingOff()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("조준 해제"));
	GetWorldTimerManager().ClearTimer(ChargingRateTimerHandle);
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
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

	// 던지는 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ThrowingMontage);
		
		// 몽타주 종료시 던지기 재활성화
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AEPCharacterPlayer::Throwing_OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowingMontage);
	}

	// 미조준 상태 - 타이머 작동X
	if (bIsAiming == false) 
	{	
		DamageMultiplier = 1.0f;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("조준 없이 던지기"));
	}
	// 조준 상태 - 타이머 작동O
	else
	{
		// 풀충전
		if ((DamageMultiplier == 2.0f))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("조준O - 최대 충전으로 던지기"));
		}
		// 애매 충전
		else 
		{
			DamageMultiplier += GetWorldTimerManager().GetTimerElapsed(ChargingRateTimerHandle) / 2.0f;
			FString str = FString::Printf(TEXT("조준O - 비례 충전으로 던지기 - 가중치 : %f"), DamageMultiplier);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, str);
		}
	}
}

void AEPCharacterPlayer::Throwing_OnMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsThrowing = false;
	if (bIsAiming)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AimingMontage);
		}

		DamageMultiplier = 1.0f;
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
		FName BombSocket(TEXT("BombHolder"));
		//BombInstance->SetOnwerCharacter(this);
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

	}
}




