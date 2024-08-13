// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosion/Character/EPCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "EPCharacterPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UInputComponent;
class UAnimMontage;
class UAnimInstance;
class USpringArmComponent;
class UCameraComponent;
class UTimelineComponent;
class UCurveFloat;
class UCameraShakeBase;
class AEPBombBase;
class UEPChargingBarWidget;

/*
 * 인게임에 사용되는 플레이어 캐릭터 클래스
*/

UCLASS()
class EXPLOSION_API AEPCharacterPlayer : public AEPCharacterBase
{
	GENERATED_BODY()

public:
	AEPCharacterPlayer();

// Replicate
public:
	/* 데미지를 입은 클라이언트의 카메라 쉐이킹 */
	UFUNCTION(client, reliable)
	void ClientRPCShakeCamera();

protected:
	/* 클라이언트에서 조준 입력 시 서버의 조준 프로퍼티 값 변경*/
	UFUNCTION(Server, Reliable)
	void ServerRPCAimingOn(bool bIsAimingOn);

	/* 서버의 조준 프로퍼티 값 변경 시 각 클라이언트에서의 조준 동작*/
	UFUNCTION()
	void OnRep_Aiming();

	/* 클라이언트에서 던지기 입력 시 서버의 던지기 프로퍼티 값 변경*/
	UFUNCTION(Server, Reliable)
	void ServerRPCThrowing();

	/* 서버의 던지기 프로퍼티 값 변경 시 각 클라이언트에서의 던지기 동작*/
	UFUNCTION()
	void OnRep_Throwing();

	/* 조준 중인지 여부 */
	UPROPERTY(ReplicatedUsing = OnRep_Aiming, EditAnyWhere, Category = "Bomb")
	uint8 bIsAiming : 1;

	/* 던지기 중인지 여부 */
	UPROPERTY(ReplicatedUsing = OnRep_Throwing, EditAnyWhere, Category = "Bomb")
	uint8 bIsThrowing : 1;


// 사망 및 리스폰
public:
	FTimerHandle DeadTimerHandle;

protected:
	/* 로비에서 본 게임으로 서버트래블 이후 지정 위치로 이동*/
	UFUNCTION()
	void TeleportToFirstSpawnPoint();

	virtual void SetDead() override;
	
	/* 사망 후 일정 시간 뒤 리스폰하며 상태 재설정*/
	void ResetPlayer();

protected:
	UPROPERTY(EditAnyWhere, Category = "Respawn")
	float RespawnTime;


// 입력
protected:
	/* 키보드 입력 캐릭터 움직임 */
	void Move(const FInputActionValue& Value);

	/* 마우스 입력 시야 움직임 */
	void Look(const FInputActionValue& Value);

	void AimingOn();
	void AimingOff();
	void Throwing();

protected:
	TObjectPtr<UInputMappingContext> InputMappingContext;

	TObjectPtr<UInputAction> Moving;

	TObjectPtr<UInputAction> Jumping;

	TObjectPtr<UInputAction> Looking;

	TObjectPtr<UInputAction> AimingBomb;

	TObjectPtr<UInputAction> ThrowingBomb;


// 애니메이션
protected:
	UFUNCTION()
	void Throwing_OnMontageEnded(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> AimingMontage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> ThrowingMontage;


// 카메라
protected:
	/* 조준 버튼 클릭에 따라 카메라 줌&아웃 진행*/
	UFUNCTION()
	void CameraZoom(float Alpha);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	/* 부드러운 카메라 줌&아웃을 위한 타임라인*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<UTimelineComponent> CameraTimelineComponent;

	/* 타임라인에 사용하는 시간별 알파값 커브*/
	UPROPERTY(EditAnyWhere, Category = "Camera")
	TObjectPtr<UCurveFloat> CameraZoomCurve;

	/* 데미지를 입었을 때 카메라가 흔들리는 정도를 가진 클래스*/
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	FOnTimelineFloat CameraZoomHandler;

	float DefaultSpringArmLength;
	float ZoomedSpringArmLength;

// 아이템
protected:
	UFUNCTION()
	virtual void TakeItem(AEPItemBase* NewItemBase) override;


// 폭탄 던지기 및 재장전
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/* 서버와 클라이언트가 동일한 재장전 동작*/
	UFUNCTION()
	virtual void OnReloadingBomb() override;

	/* 서버와 클라이언트가 동일한 던지기 동작을 한다. 서버에서는 추가적으로 폭탄의 던지기가 진행된다*/
	UFUNCTION()
	virtual void OnThrowingBomb() override;


protected:
	FTimerHandle ChargingRateTimerHandle;
	
	/* 플레이어가 차징을 진행한 시간*/
	float ChargingDuration;

	/* 폭탄 매니저에서 가져와 던지기에 사용되는 폭탄 */
	UPROPERTY(VisibleAnyWhere, Category = "Bomb")
	TObjectPtr<AEPBombBase> BombToThrow;

	/* 캐릭터가 시작부터 끝까지 손에 들고있는 폭탄 */
	UPROPERTY(VisibleAnyWhere, Category = "Bomb")
	TObjectPtr<AEPBombBase> BombInHand;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	FVector ThrowingPower;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	float ThrowingVelocityMultiplier;

	float ThrowingVelocityMultiplierDefault;
	float ThrowingVelocityMultiplierMax;

// UI Section
	TSubclassOf<UEPChargingBarWidget> ChargingBarWidgetClass;
	TObjectPtr<UEPChargingBarWidget> ChargingBarWidget;


protected:
	TObjectPtr<APlayerController> PlayerController;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
