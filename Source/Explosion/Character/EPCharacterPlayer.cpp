// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Explosion/Character/EPCharacterPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/VerticalBox.h"
#include "Camera/CameraComponent.h"
#include "Explosion/Animation/EPPlayerAnimInstance.h"
#include "Explosion/Bomb/EPBombBase.h"
#include "Explosion/Bomb/EPBombManager.h"
#include "Explosion/Stat/EPCharacterStatComponent.h"
#include "Explosion/GameData/EPDeathMatchGameMode.h"
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/Item/EPItemBase.h"
#include "Explosion/UI/EPHUDWidget.h"
#include "Explosion/UI/EPChargingBarWidget.h"
#include "Explosion/EPCharacterStat.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Explosion/Explosion.h"

// EP_LOG(LogExplosion, Log, TEXT("%s"), TEXT("로그 참고용"));

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

	CameraTimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("CameraTimelineComponent"));

	// 에셋 로드 - 카메라
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CameraZoomCurveFinder
	(TEXT("/Game/Character/Curves/CT_CameraZoom.CT_CameraZoom"));
	if (CameraZoomCurveFinder.Succeeded())
	{
		CameraZoomCurve = CameraZoomCurveFinder.Object;
	}

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

	// 에셋 로드 - UI
	static ConstructorHelpers::FClassFinder<UUserWidget> ChargingBarWidgetFinder
	(TEXT("/Game/UI/WBP_ChargingBarWidget.WBP_ChargingBarWidget_C"));
	if (ChargingBarWidgetFinder.Class)
	{
		ChargingBarWidgetClass = ChargingBarWidgetFinder.Class;
	}

	// 카메라 설정
	DefaultSpringArmLength = 350.0f;
	ZoomedSpringArmLength = 120.0f;

	// 폭탄 설정
	ThrowingVelocityMultiplierDefault = 1.0f;
	ThrowingVelocityMultiplierMax = ThrowingVelocityMultiplierDefault + 1.0f;
	ThrowingVelocityMultiplier = ThrowingVelocityMultiplierDefault;

	ThrowingVelocity = 500.0f;

	bIsAiming = false;
	bIsThrowing = false;

	RespawnTime = 3.0f;

	ChargingDuration = 2.0f;
}

void AEPCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 입력 컨텍스트 지정
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr) {
			Subsystem->AddMappingContext(InputMappingContext, 1);
		}
	}

	// 카메라 줌 설정
	CameraZoomHandler.BindUFunction(this, FName("CameraZoom"));
	CameraTimelineComponent->AddInterpFloat(CameraZoomCurve, CameraZoomHandler);

	AnimInstance = GetMesh()->GetAnimInstance();

	// 들고 있을 폭탄 스폰 및 소켓에 부착
	if (HasAuthority())
	{
		BombInHand = GetWorld()->SpawnActor<AEPBombBase>(BP_Bomb, FTransform::Identity);
		if (BombInHand)
		{
			BombInHand->SetBombInHandOption();
			FName BombSocket(TEXT("BombHolder"));
			if (GetMesh()->DoesSocketExist(BombSocket))
			{
				BombInHand->AttachToComponent(
					GetMesh(),
					FAttachmentTransformRules::SnapToTargetIncludingScale,
					BombSocket
				);
			}
			BombMass = BombInHand->GetBombMass();
		}
	}

	// 차징 바 위젯 생성
	if (IsLocallyControlled())
	{
		ChargingBarWidget = CreateWidget<UEPChargingBarWidget>(GetWorld(), ChargingBarWidgetClass);
		HUDWidget->ChargingBarFrame->AddChild(ChargingBarWidget);
		ChargingBarWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 사망 바인딩
	StatComponent->OnHpZero.AddUObject(this, &AEPCharacterPlayer::SetDead);

	// 폭탄을 던지고 재장전 할 때의 대리자 연결
	OnThrowingBombDelegate.BindUObject(this, &AEPCharacterPlayer::OnThrowingBomb);
	OnReloadingBombDelegate.BindUObject(this, &AEPCharacterPlayer::OnReloadingBomb);
}

void AEPCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEPCharacterPlayer, bIsAiming);
	DOREPLIFETIME(AEPCharacterPlayer, bIsThrowing);
}

void AEPCharacterPlayer::SetDead()
{
	Super::SetDead();
	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//GetMesh()->SetSimulatePhysics(true);
	if (BombInHand)
	{
		BombInHand->SetActorHiddenInGame(true);
	}
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	DisableInput(PlayerController);

	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &AEPCharacterPlayer::ResetPlayer, RespawnTime, false);
}

void AEPCharacterPlayer::ResetPlayer()
{
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);
	}

	if (GetWorld()->GetGameState<AEPGameState>()->GetIsSetupEndMatch() == false)
	{
		EnableInput(PlayerController);
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	if (BombInHand)
	{
		BombInHand->SetActorHiddenInGame(false);
	}

	StatComponent->ResetHp();

	if (HasAuthority())
	{
		AEPDeathMatchGameMode* GameMode = Cast<AEPDeathMatchGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			FTransform NewTransform = GameMode->GetRandomStartTransform();
			TeleportTo(NewTransform.GetLocation(), NewTransform.GetRotation().Rotator());
		}
	}

}


void AEPCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 궤적 그리기 테스트
	//if (IsLocallyControlled())
	//{
	//	if (!bIsThrowing)
	//	{
	//		FRotator ThrowingRotation = GetControlRotation();
	//		FVector ThrowingDirection = ThrowingRotation.Vector();
	//		FRotator BombInHandRotation = BombInHand->GetActorRotation();
	//		FVector BombInHandLocation = BombInHand->GetActorLocation();
	//		ThrowingPower = ThrowingDirection * ThrowingVelocityMultiplier * ThrowingVelocity * BombMass;
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("ThrowingPower : %s"), *(ThrowingDirection*1000).ToString()));
	//		DrawThrowingPath();
	//	}
	//}
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
	// 서버, 클라 공통 영역
	if (bIsThrowing)
	{
		return;
	}
	CameraTimelineComponent->Play();
	ServerRPCAimingOn(true);
}

void AEPCharacterPlayer::AimingOff()
{
	CameraTimelineComponent->Reverse();
	ServerRPCAimingOn(false);
}

void AEPCharacterPlayer::ServerRPCAimingOn_Implementation(bool bIsAimingOn)
{
	bIsAiming = bIsAimingOn;
	
	OnRep_Aiming();
}

void AEPCharacterPlayer::TakeItem(AEPItemBase* NewItemBase)
{
	if (NewItemBase)
	{
		switch (NewItemBase->Type)
		{
		case EItemType::IT_Health:
			float ItemHpValue = NewItemBase->ItemStat.MaxHp;
			StatComponent->RecoverHp(ItemHpValue);
			break;
		}
	}
}

void AEPCharacterPlayer::OnRep_Aiming()
{
	if (AnimInstance)
	{
		// 조준
		if (bIsAiming)
		{
			AnimInstance->Montage_Play(AimingMontage);
			if (IsLocallyControlled() && ChargingBarWidget)
			{
				ChargingBarWidget->SetVisibility(ESlateVisibility::Visible);
				ChargingBarWidget->StartChargingBar(ChargingDuration);
			}
			if (HasAuthority())
			{
				GetWorldTimerManager().SetTimer(ChargingRateTimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						ThrowingVelocityMultiplier = ThrowingVelocityMultiplierMax;
					}
				), ChargingDuration, false);
			}
		}
		// 조준 해제
		else
		{
			AnimInstance->Montage_Stop(0.3f, AimingMontage);

			if (IsLocallyControlled() && ChargingBarWidget)
			{
				ChargingBarWidget->EndChargingBar();
				ChargingBarWidget->SetVisibility(ESlateVisibility::Visible);
			}
			if (HasAuthority())
			{
				GetWorldTimerManager().ClearTimer(ChargingRateTimerHandle);
				ThrowingVelocityMultiplier = ThrowingVelocityMultiplierDefault;
			}
		}
	}
}

void AEPCharacterPlayer::CameraZoom(float Alpha)
{
	SpringArmComponent->TargetArmLength = FMath::Lerp(DefaultSpringArmLength, ZoomedSpringArmLength, Alpha);
}

void AEPCharacterPlayer::Throwing()
{
	if (bIsThrowing) 
	{
		return;
	}
	ServerRPCThrowing();
}


void AEPCharacterPlayer::ServerRPCThrowing_Implementation()
{
	bIsThrowing = true;
	OnRep_Throwing();
}

void AEPCharacterPlayer::OnRep_Throwing()
{
	if (bIsThrowing)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(ThrowingMontage);
			FOnMontageBlendingOutStarted BlendingOutDelegate;
			BlendingOutDelegate.BindUObject(this, &AEPCharacterPlayer::Throwing_OnMontageEnded);
			AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, ThrowingMontage);
		}

		if (IsLocallyControlled() && ChargingBarWidget)
		{
			ChargingBarWidget->EndChargingBar();
		}

		if (HasAuthority())
		{
			if (bIsAiming == false)
			{
				ThrowingVelocityMultiplier = ThrowingVelocityMultiplierDefault;
			}
			else if (ThrowingVelocityMultiplier < ThrowingVelocityMultiplierMax)
			{
				float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ChargingRateTimerHandle);
				ThrowingVelocityMultiplier = ThrowingVelocityMultiplierDefault * (1.0 + (ElapsedTime / ChargingDuration));
			}
			else
			{
				ThrowingVelocityMultiplier = ThrowingVelocityMultiplierMax;
			}
		}
	}
}


void AEPCharacterPlayer::Throwing_OnMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (bIsAiming)
	{
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AimingMontage);
		}

		if (IsLocallyControlled() && ChargingBarWidget)
		{
			ChargingBarWidget->StartChargingBar(ChargingDuration);
		}

		if (HasAuthority())
		{
			ThrowingVelocityMultiplier = ThrowingVelocityMultiplierDefault;

			GetWorldTimerManager().ClearTimer(ChargingRateTimerHandle);
			GetWorldTimerManager().SetTimer(ChargingRateTimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					ThrowingVelocityMultiplier = ThrowingVelocityMultiplierMax;
				}
			), ChargingDuration, false);
		}
	}

	if (HasAuthority())
	{
		bIsThrowing = false;
	}
}

void AEPCharacterPlayer::OnReloadingBomb()
{
	if (BombInHand)
	{
		BombInHand->SetActorHiddenInGame(false);
	}
}

void AEPCharacterPlayer::OnThrowingBomb()
{
	if (BombInHand)
	{
		BombInHand->SetActorHiddenInGame(true);
	}

	if (HasAuthority())
	{
		FRotator ThrowingRotation = GetControlRotation();
		FVector ThrowingDirection = ThrowingRotation.Vector();
		FRotator BombInHandRotation = BombInHand->GetActorRotation();
		FVector BombInHandLocation = BombInHand->GetActorLocation();

		//BombToThrow = GetWorld()->SpawnActor<AEPBombBase>(BP_Bomb, BombInHandLocation, BombInHandRotation);
		BombToThrow = BombManager->TakeBomb();
		if (BombToThrow)
		{
			BombToThrow->SetActorLocationAndRotation(BombInHandLocation, BombInHandRotation);
			ThrowingPower = ThrowingDirection * ThrowingVelocityMultiplier * ThrowingVelocity * BombMass;
			BombToThrow->GetBombMeshComponent()->AddImpulse(ThrowingPower);
		}
	}
}

// 폭탄 던지기 경로 예측
//void AEPCharacterPlayer::DrawThrowingPath()
//{
//	FPredictProjectilePathParams Params(
//		0.0f,
//		BombInHand->GetActorLocation(),
//		ThrowingPower,
//		15.0f,
//		ECollisionChannel::ECC_Visibility
//	);
//	Params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
//	Params.OverrideGravityZ = -980.0f * BombMass;
//
//	FPredictProjectilePathResult Result;
//	UGameplayStatics::PredictProjectilePath(this, Params, Result);
//}






