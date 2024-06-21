// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBombManager.h"

// Sets default values for this component's properties
UEPBombManager::UEPBombManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicated(true);

	MaxBombCount = 4;
	CurrentBombCount = 4;
}


// Called when the game starts
void UEPBombManager::BeginPlay()
{
	Super::BeginPlay();	
}


void UEPBombManager::MakeBombObjectPool(TSubclassOf<AEPBombBase> BP_Bomb)
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (int i = 0; i < MaxBombCount; ++i)
		{
			AEPBombBase* PoolableBomb = World->SpawnActor<AEPBombBase>(BP_Bomb, FVector().ZeroVector, FRotator().ZeroRotator);
			PoolableBomb->SetOwner(GetOwner());
			BombList.Add(PoolableBomb);
		}
	}
}

TObjectPtr<AEPBombBase> UEPBombManager::TakeBomb()
{
	// 이후 구조 변경 -> 잔여 폭탄 및 전체 폭탄 수량 체크
	for (TObjectPtr<AEPBombBase> Bomb : BombList)
	{
		if (!Bomb->GetIsBombActive())
		{
			Bomb->ActiveBomb();
			Bomb->ActiveBombTimeTrigger();
			return Bomb;
		}
	}
	return nullptr;
}

// 자동 추가 기능은 이후 추가
