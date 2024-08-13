// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Explosion/EPCharacterStat.h"
#include "EPCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnHpZeroThreeParamsDelegate, AController* /*KillerPlayer*/, AController* /*KilledPlayer*/, AActor* /*DamageCauser*/);
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);

/**
* 플레이어의 스텟을 관리하기 위한 컴포넌트. 현재는 HP만 관리한다.
* 플레이어가 데미지를 입었을 때, HP를 변경하고 이를 대리자를 통해 알리는 역할을 수행한다. 
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXPLOSION_API UEPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEPCharacterStatComponent();

// 대리자
public:
	/* 단순히 HP가 변경되었음을 알리는 대리자 */
	FOnHpChangedDelegate OnHpChanged;

	/* HP가 0이 되었을 때 Kill Death를 업데이트하기 위한 대리자 */
	FOnHpZeroThreeParamsDelegate OnHpZeroThreeParams;

	/* HP가 0이 되었을 때 캐릭터 사망 처리를 하기 위한 대리자 */
	FOnHpZeroDelegate OnHpZero;

// Replicate
protected:
	/* 현재 체력이 변경되었을 때, 대리자를 통해 이를 알림*/
	UFUNCTION()
	void OnRep_CurrentHp();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;


public:
	float TakeDamage(float Damage, AController* EventCauser, AController* EventInstigator, AActor* DamageCauser);
	
	/* 게임 인스턴스에 저장된 기본 스텟을 불러와 캐릭터에 적용 */
	void SetBaseStat();

	/* 현재 HP를 최대 HP로 변경 */
	void ResetHp();

	void RecoverHp(float Amount);

	float GetMaxHp() const { return MaxHp; }
	float GetCurrentHp() const { return CurrentHp; }


protected:
	/* 현재 체력을 주어진 값으로 업데이트 */
	void SetHp(float NewHp);


// Stat Section
protected:
	/* 게임 인스턴스로부터 가져온 기본 설정 스텟 */
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	FEPCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHp;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
