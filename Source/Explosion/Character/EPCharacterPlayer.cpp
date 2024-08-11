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
#include "LegacyCameraShake.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"


AEPCharacterPlayer::AEPCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 관련 컴포넌트 설정
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// 줌&아웃용 타임라인
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
	static ConstructorHelpers::FClassFinder<UEPChargingBarWidget> ChargingBarWidgetFinder
	(TEXT("/Game/UI/WBP_ChargingBarWidget.WBP_ChargingBarWidget_C"));
	if (ChargingBarWidgetFinder.Class)
	{
		ChargingBarWidgetClass = ChargingBarWidgetFinder.Class;
	}

	// 에셋 로드 - 카메라 쉐이크
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CameraShakeClassFinder
	(TEXT("/Game/Animation/CameraShake/BP_Bomb_CS.BP_Bomb_CS_C"));
	if (CameraShakeClassFinder.Class)
	{
		CameraShakeClass = CameraShakeClassFinder.Class;
	}

	// 카메라 설정
	DefaultSpringArmLength = 350.0f;
	ZoomedSpringArmLength = 120.0f;

	// 폭탄을 어느 정도로 던질지 설정
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

	AnimInstance = GetMesh()->GetAnimInstance();
	
	// 입력 컨텍스트 지정
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr) 
		{
			Subsystem->AddMappingContext(InputMappingContext, 1); // EPPlayerController의 우선순위는 0이다.
		}
	}

	// 카메라 줌 설정
	CameraZoomHandler.BindUFunction(this, FName("CameraZoom"));
	CameraTimelineComponent->AddInterpFloat(CameraZoomCurve, CameraZoomHandler);

	if (HasAuthority())
	{
		// 첫 시작 위치 지정
		FTimerHandle TeleportToFirstSpawnPointTimerHandle;
		GetWorldTimerManager().SetTimer(TeleportToFirstSpawnPointTimerHandle, this, &AEPCharacterPlayer::TeleportToFirstSpawnPoint, 2.0f, false);

		// 캐릭터가 손에 들고있는 폭탄 생성 및 설정
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

	if (IsLocallyControlled())
	{
		// UI 등록 및 설정
		ChargingBarWidget = CreateWidget<UEPChargingBarWidget>(GetWorld(), ChargingBarWidgetClass);
		HUDWidget->ChargingBarFrame->AddChild(ChargingBarWidget);
		ChargingBarWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 사망시 발동하는 대리자 연결
	if (StatComponent)
	{
		StatComponent->OnHpZero.AddUObject(this, &AEPCharacterPlayer::SetDead);
	}

	// 폭탄 던지기 및 재장전 시 발동하는 대리자 연결
	OnThrowingBombDelegate.BindUObject(this, &AEPCharacterPlayer::OnThrowingBomb);
	OnReloadingBombDelegate.BindUObject(this, &AEPCharacterPlayer::OnReloadingBomb);
}

void AEPCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEPCharacterPlayer, bIsAiming);
	DOREPLIFETIME(AEPCharacterPlayer, bIsThrowing);
}

float AEPCharacterPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	ClientRPCShakeCamera();
	return 0.0f;
}

void AEPCharacterPlayer::TeleportToFirstSpawnPoint()
{
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

void AEPCharacterPlayer::SetDead()
{
	Super::SetDead();

	if (BombInHand)
	{
		BombInHand->SetActorHiddenInGame(true);
	}

	if (PlayerController)
	{
		DisableInput(PlayerController);
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// 지정 시간이 흐른 후 플레이어 리셋(리스폰)
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

	// 리스폰 위치로 이동
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

void AEPCharacterPlayer::ClientRPCShakeCamera_Implementation()
{
	if (IsLocallyControlled())
	{
		if (CameraShakeClass)
		{
			float ShakeScale = 1.0f;
			PlayerController->ClientStartCameraShake(CameraShakeClass, ShakeScale);
		}
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

		// 축
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
	CameraTimelineComponent->Play();

	// 조준 여부를 서버에 알림
	ServerRPCAimingOn(true);
}

void AEPCharacterPlayer::AimingOff()
{
	CameraTimelineComponent->Reverse();

	// 조준 해제 여부를 서버에 알림
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
			if (StatComponent)
			{
				StatComponent->RecoverHp(ItemHpValue);
			}
			break;
		}
	}
}

void AEPCharacterPlayer::OnRep_Aiming()
{
	// 조준 애니메이션 및 UI 처리
	if (AnimInstance)
	{
		// 조준 시작
		if (bIsAiming)
		{
			AnimInstance->Montage_Play(AimingMontage);
			// 클라이언트 UI 처리
			if (IsLocallyControlled() && ChargingBarWidget)
			{
				ChargingBarWidget->SetVisibility(ESlateVisibility::Visible);
				ChargingBarWidget->StartChargingBar(ChargingDuration);
			}
			// 서버 폭탄 차징 시작
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
			// 클라이언트 UI 처리
			if (IsLocallyControlled() && ChargingBarWidget)
			{
				ChargingBarWidget->EndChargingBar();
				ChargingBarWidget->SetVisibility(ESlateVisibility::Collapsed);
			}
			// 서버 폭탄 차징 종료
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
	if (SpringArmComponent)
	{
		SpringArmComponent->TargetArmLength = FMath::Lerp(DefaultSpringArmLength, ZoomedSpringArmLength, Alpha);
	}
}

void AEPCharacterPlayer::Throwing()
{
	if (bIsThrowing) 
	{
		return;
	}

	// 서버에 폭탄 던짐 여부를 전달
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

			// 이번 몽타주 재생이 끝났을 때 호출할 함수 지정
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
			// 미조준 상태에서 폭탄을 던질 경우 기본값 사용
			if (bIsAiming == false)
			{
				ThrowingVelocityMultiplier = ThrowingVelocityMultiplierDefault;
			}
			// 조준 중 폭탄을 던질 경우 가중값 사용
			else if (ThrowingVelocityMultiplier < ThrowingVelocityMultiplierMax)
			{
				float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(ChargingRateTimerHandle);
				ThrowingVelocityMultiplier = ThrowingVelocityMultiplierDefault * (1.0 + (ElapsedTime / ChargingDuration));
			}
			// 조준 중 최대 차징 이후 폭탄을 던질 경우 최대값 사용 
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

	// 서버에서 폭탄 던지기 처리
	if (HasAuthority())
	{
		FRotator ThrowingRotation = GetControlRotation();
		FVector ThrowingDirection = ThrowingRotation.Vector();
		FRotator BombInHandRotation = BombInHand->GetActorRotation();
		FVector BombInHandLocation = BombInHand->GetActorLocation();

		BombToThrow = BombManager->TakeBomb();
		if (BombToThrow)
		{
			BombToThrow->SetActorLocationAndRotation(BombInHandLocation, BombInHandRotation);
			ThrowingPower = ThrowingDirection * ThrowingVelocityMultiplier * ThrowingVelocity * BombMass;
			BombToThrow->GetBombMeshComponent()->AddImpulse(ThrowingPower);
		}
	}
}



