// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EPBombBase.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UAudioComponent;
class UGameplayStatics;
class UParticleSystem;
class UEPBombManager;

UCLASS()
class EXPLOSION_API AEPBombBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AEPBombBase();

// Replicate
protected:
	/* 모든 클라이언트가 이펙트를 재생하고, 서버는 데미지 처리까지 진행*/
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCExplode();

// Set & Get 
public:
	void SetBombOnwer(TObjectPtr<AActor> NewBombOwner) { BombOwner = NewBombOwner; }
	bool GetIsBombActive() const { return bIsBombActive; }
	float GetBombMass() const { return BombMass; }
	TObjectPtr<UStaticMeshComponent> GetBombMeshComponent() const { return BombMeshComponent; }

// 폭탄 활성화 & 비활성화 옵션 조정 함수
public:
	void ActiveBomb();
	void DeactiveBomb();

	/* 플레이어 캐릭터가 손에 들고있는 폭탄의 초기 설정 실행*/
	void SetBombInHandOption();
	void ActiveBombTimeTrigger();

// 컴포넌트	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> BombOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> BombMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> BombAudioComponent;

// 폭탄이 터졌을 때 재생될 파티클
protected:
	UPROPERTY()
	TObjectPtr<UGameplayStatics> BombGamePlayStatics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> BombParticleSystem;

// 폭탄을 관리할 매니저
public:
	UPROPERTY()
	TObjectPtr<UEPBombManager> BombManager;

// 폭탄 스탯
protected:
	UPROPERTY(EditAnyWhere, Category = "BombStat")
	float BombMass;

	UPROPERTY(EditAnyWhere, Category = "BombStat")
	float BombDamage;

	UPROPERTY(EditAnyWhere, Category = "BombStat")
	float BombAreaRadius;

	UPROPERTY(EditAnyWhere, Category = "BombStat")
	float BombDelayTime;

	uint8 bIsBombActive : 1;


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
