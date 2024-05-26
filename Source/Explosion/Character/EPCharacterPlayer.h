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

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// PlayerController Section
protected:
	TObjectPtr<APlayerController> PlayerController;


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

	FOnTimelineFloat CameraZoomHandler;
	float DefaultSpringArmLength;
	float ZoomedSpringArmLength;


// Bomb Section
protected:
	UFUNCTION()
	virtual void OnReloadingBomb() override;

	UFUNCTION()
	virtual void OnThrowingBomb() override;

	//UFUNCTION()
	//void DrawThrowingPath();

protected:
	FTimerHandle ChargingRateTimerHandle;

	UPROPERTY(VisibleAnyWhere, Category = "Bomb")
	TObjectPtr<class AEPBombBase> BombToThrow;

	UPROPERTY(VisibleAnyWhere, Category = "Bomb")
	TObjectPtr<class AEPBombBase> BombInHand;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	FVector ThrowingPower;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	float ThrowingDistanceMultiplier;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	uint8 bIsAiming : 1;

	UPROPERTY(EditAnyWhere, Category = "Bomb")
	uint8 bIsThrowing : 1;
};
