// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Explosion/Interface/EpCharacterItemInterface.h"
#include "EPCharacterBase.generated.h"

DECLARE_DELEGATE(FOnThrowingBombDelegate);
DECLARE_DELEGATE(FOnReloadingBombDelegate);

UCLASS()
class EXPLOSION_API AEPCharacterBase : public ACharacter, public IEPCharacterItemInterface
{
	GENERATED_BODY()

public:
	AEPCharacterBase();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// Delegate Section
public:
	FOnThrowingBombDelegate OnThrowingBombDelegate;
	FOnReloadingBombDelegate OnReloadingBombDelegate;

// Interface Section
public:
	virtual void TakeItem(class AEPItemBase* NewItemBase) override;

// Damage Section
protected:
	virtual void SetDead();
	virtual void TempSetDamaged(float CurrentHp, float MaxHp);
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


// Bomb Manager Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombManager", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEPBombManager> BombManager;

// Bomb Section
protected:
	UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEPBombBase> BP_Bomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	float ThrowingVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	float BombMass;

	virtual void OnThrowingBomb();
	virtual void OnReloadingBomb();

// Timer
public:
	UFUNCTION()
	void StartMainGame();


protected: 
	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerReady();


// UI
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> OverHeadWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEPOverHeadWidget> OverHeadWidget;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEPHUDWidget> HUDWidget;
	TSubclassOf<class UEPHUDWidget> HUDWidgetClass;
// Stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEPCharacterStatComponent> StatComponent;
};
