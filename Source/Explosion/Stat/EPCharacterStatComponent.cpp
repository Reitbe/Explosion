// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacterStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "Explosion/GameData/EPGameInstance.h"


UEPCharacterStatComponent::UEPCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	SetBaseStat();
}

void UEPCharacterStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UEPCharacterStatComponent, CurrentHp);
}

void UEPCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

float UEPCharacterStatComponent::TakeDamage(float Damage, AController* EventCauser, AController* EventInstigator, AActor* DamageCauser)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(Damage, 0, Damage);

	// 데미지를 적용한 체력으로 현재 체력 변경
	SetHp(CurrentHp - ActualDamage);;

	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
		OnHpZeroThreeParams.Broadcast(EventInstigator, EventCauser, DamageCauser);
	}
	return ActualDamage;
}

void UEPCharacterStatComponent::ResetHp()
{
	SetHp(MaxHp);
}

void UEPCharacterStatComponent::RecoverHp(float Amount)
{
	SetHp(CurrentHp + Amount);
}

void UEPCharacterStatComponent::SetBaseStat()
{
	UEPGameInstance* GameInstance = Cast<UEPGameInstance>(GetWorld()->GetGameInstance());
	BaseStat = GameInstance->GetDefaultCharacterStat();
	MaxHp = BaseStat.MaxHp;
	SetHp(MaxHp);
}

void UEPCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0, MaxHp);
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UEPCharacterStatComponent::OnRep_CurrentHp()
{
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
	if (CurrentHp <= 0)
	{
		OnHpZero.Broadcast();
	}
}
