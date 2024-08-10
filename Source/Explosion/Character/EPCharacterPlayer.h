// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosion/Character/EPCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "EPCharacterPlayer.generated.h"

UCLASS()
class EXPLOSION_API AEPCharacterPlayer : public AEPCharacterBase
{
	GENERATED_BODY()


public:
	AEPCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(client, reliable)
	void ClientRPC_ShakeCamera();

// PlayerController Section
protected:
	TObjectPtr<APlayerController> PlayerController;

// Dead and Respawn Section
public:
	FTimerHandle DeadTimerHandle;

protected:
	virtual void SetDead() override;
	void ResetPlayer();


// Input Section
protected:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> Moving;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> Jumping;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> Looking;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> AimingBomb;

	UPROPERTY(EditAnyWhere, Category = "Enhanced Input")
	TObjectPtr<class UInputAction> ThrowingBomb;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void AimingOn();
	void AimingOff();
	void Throwing();

// Animation Section
	UFUNCTION()
	void Throwing_OnMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimInstance> AnimInstance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> AimingMontage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<class UAnimMontage> ThrowingMontage;


// Camera Section
protected:
	UFUNCTION()
	void CameraZoom(float Alpha);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	TObjectPtr<class UTimelineComponent> CameraTimelineComponent;

	UPROPERTY(EditAnyWhere, Category = "Camera")
	TObjectPtr<class UCurveFloat> CameraZoomCurve;

	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	FOnTimelineFloat CameraZoomHandler;
	float DefaultSpringArmLength;
	float ZoomedSpringArmLength;

// Item Section
protected:
	UFUNCTION()
	virtual void TakeItem(AEPItemBase* NewItemBase) override;


// Aiming & Throwing Section
protected:
	UFUNCTION(Server, Reliable)
	void ServerRPCAimingOn(bool bIsAimingOn);

	UFUNCTION()
	void OnRep_Aiming();

	UFUNCTION()
	virtual void OnReloadingBomb() override;

	UFUNCTION()
	virtual void OnThrowingBomb() override;

	UFUNCTION(Server, Reliable)
	void ServerRPCThrowing();

	UFUNCTION()
	void OnRep_Throwing();

	float ChargingDuration;

	//UFUNCTION()
	//void DrawThrowingPath();

// Bomb Section
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	FTimerHandle ChargingRateTimerHandle;

	UPROPERTY(VisibleAnyWhere, Category = "Bomb")
	TObjectPtr<class AEPBombBase> BombToThrow;

	UPROPERTY(VisibleAnyWhere, Category = "Bomb")
	TObjectPtr<class AEPBombBase> BombInHand;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	FVector ThrowingPower;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	float ThrowingVelocityMultiplier;

	float ThrowingVelocityMultiplierDefault;
	float ThrowingVelocityMultiplierMax;

	UPROPERTY(ReplicatedUsing = OnRep_Aiming, EditAnyWhere, Category = "Bomb")
	uint8 bIsAiming : 1;

	UPROPERTY(ReplicatedUsing = OnRep_Throwing, EditAnyWhere, Category = "Bomb")
	uint8 bIsThrowing : 1;

// UI Section
	TSubclassOf<class UEPChargingBarWidget> ChargingBarWidgetClass;
	TObjectPtr<class UEPChargingBarWidget> ChargingBarWidget;

// Respawn Section
protected:
	UFUNCTION()
	void TeleportToFirstSpawnPoint();

protected:
	UPROPERTY(EditAnyWhere, Category = "Respawn")
	float RespawnTime;

};
