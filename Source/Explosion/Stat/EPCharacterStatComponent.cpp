// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacterStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Explosion/Explosion.h"
// EP_SUBLOG(LogExplosion, Log, TEXT("%s"), TEXT("전체 다 몽타주를 재생"));

// Sets default values for this component's properties
UEPCharacterStatComponent::UEPCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	MaxHp = 100.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UEPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEPCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEPCharacterStatComponent, CurrentHp);
}


// Called every frame
void UEPCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UEPCharacterStatComponent::TakeDamage(float Damage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(Damage, 0, Damage);
	SetHp(CurrentHp - ActualDamage);;
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UEPCharacterStatComponent::ResetHp()
{
	SetHp(MaxHp);
}

void UEPCharacterStatComponent::SetHp(float NewHp)
{
	EP_SUBLOG(LogExplosion, Log, TEXT("%s"), TEXT("체력변경됨"));
	CurrentHp = FMath::Clamp<float>(NewHp, 0, MaxHp);
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UEPCharacterStatComponent::OnRep_CurrentHp()
{
	EP_SUBLOG(LogExplosion, Log, TEXT("%s"), TEXT("체력변경되어서 OnRep도 실행됨."));
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
	if (CurrentHp <= 0)
	{
		OnHpZero.Broadcast();
	}
}
