// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Explosion/EPCharacterStat.h"
#include "EPCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float /*CurrentHp*/, float /*MaxHp*/);
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXPLOSION_API UEPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEPCharacterStatComponent();

// Delegate Section
public:
	FOnHpChangedDelegate OnHpChanged;
	FOnHpZeroDelegate OnHpZero;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// 
public:
	void SetBaseStat();
	float TakeDamage(float Damage);

	void ResetHp();
	void RecoverHp(float Amount);

	float GetMaxHp() const { return MaxHp; }
	float GetCurrentHp() const { return CurrentHp; }

		// Function Section
protected:
	void SetHp(float NewHp);

	UFUNCTION()
	void OnRep_CurrentHp();
	
// Stat Section
protected:
	UPROPERTY(EditDefaultsOnly, Category = Stat)
	FEPCharacterStat BaseStat;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp, Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHp;



};
