// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Explosion/Interface/EpCharacterItemInterface.h"
#include "EPCharacterBase.generated.h"

class UInputComponent;
class AEPItemBase;
class UEPBombManager;
class AEPBombBase;
class UWidgetComponent;
class UEPOverHeadWidget;
class UEPHUDWidget;
class UEPCharacterStatComponent;

DECLARE_DELEGATE(FOnThrowingBombDelegate);
DECLARE_DELEGATE(FOnReloadingBombDelegate);

/*
 * 다양한 플레이어 캐릭터들의 기본 클래스
*/

UCLASS()
class EXPLOSION_API AEPCharacterBase : public ACharacter, public IEPCharacterItemInterface
{
	GENERATED_BODY()

public:
	AEPCharacterBase();

// 대리자
public:
	/* 폭탄을 던지는 시점에 AnimNotify에서 대리자가 호출된다 */
	FOnThrowingBombDelegate OnThrowingBombDelegate;

	/* 폭탄을 재장전하는 시점에 AnimNotify에서 대리자가 호출된다 */
	FOnReloadingBombDelegate OnReloadingBombDelegate;


// Replicate
protected:
	/* 캐릭터가 준비되었음을 서버에 알린다 */
	UFUNCTION(Server, Reliable)
	void ServerRPCPlayerReady();


// IEPCharacterItemInterface
public:
	virtual void TakeItem(class AEPItemBase* NewItemBase) override;


// 데미지 처리
protected:
	virtual void SetDead();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


// 폭탄 오브젝트풀을 관리하는 폭탄 매니저
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BombManager", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEPBombManager> BombManager;


// 캐릭터가 던질 폭탄
protected:
	/* 폭탄을 던졌을 때의 내부 동작 */
	virtual void OnThrowingBomb();

	/* 폭탄을 재장전했을 때의 내부 동작 */
	virtual void OnReloadingBomb();

protected:
	UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEPBombBase> BP_Bomb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	float ThrowingVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bomb", Meta = (AllowPrivateAccess = "true"))
	float BombMass;


// 본 게임 시작
public:
	/* 모든 캐릭터가 준비된 이후 본 게임이 시작될 때 실행되는 함수*/
	UFUNCTION()
	void StartMainGame();

// UI
protected:
	void SetOverHeadPlayerNameUI();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> OverHeadWidgetComponent;

	TObjectPtr<UEPOverHeadWidget> OverHeadWidget;

	TSubclassOf<UEPHUDWidget> HUDWidgetClass;
	TObjectPtr<UEPHUDWidget> HUDWidget;

// Stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEPCharacterStatComponent> StatComponent;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

};
