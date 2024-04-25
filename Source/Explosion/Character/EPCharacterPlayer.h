// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosion/Character/EPCharacterBase.h"
#include "InputActionValue.h"
#include "EPCharacterPlayer.generated.h"

/**
 * 
 */
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

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);


// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> Camera;

	
};
