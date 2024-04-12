// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "DefaultCharacter.generated.h"


UCLASS()
class EXPLOSION_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADefaultCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// PlayerController Section
protected:
	APlayerController* PlayerController;


// Input Section
protected:
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, Category = "Enhanced Input")
	class UInputAction* Moving;

	void Move(const FInputActionValue& Value);


// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* Camera;
};
