 // Fill out your copyright notice in the Description page of Project Settings.


#include "EPBombManager.h"

// Sets default values for this component's properties
UEPBombManager::UEPBombManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UEPBombManager::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);
	
	MaxBombCount = 4;
	CurrentBombCount = 4;
}


void UEPBombManager::MakeBombObjectPool(TSubclassOf<AEPBombBase> BP_Bomb)
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < MaxBombCount; ++i)
		{
			AEPBombBase* PoolableBomb = World->SpawnActor<AEPBombBase>(BP_Bomb, FVector().ZeroVector, FRotator().ZeroRotator);
			BombList.Add(PoolableBomb);
			PoolableBomb->SetOwner(GetOwner()); // 폭탄을 소유한 플레이어로 오너 설정
		}
	}
}

TObjectPtr<AEPBombBase> UEPBombManager::TakeBomb()
{
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

