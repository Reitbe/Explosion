// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPBombBase.h"
#include "EPBombManager.generated.h"


UCLASS()
class EXPLOSION_API UEPBombManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEPBombManager();

public:
	/** 주어진 폭탄 종류로 오브젝트 풀 생성 */
	void MakeBombObjectPool(TSubclassOf<AEPBombBase> BP_Bomb);
	
	/** 오브젝트 풀에서 사용 가능한 폭탄을 가져온다 */
	TObjectPtr<AEPBombBase> TakeBomb();

protected:
	/** 폭탄들이 담겨있는 오브젝트 풀 */
	TArray<TObjectPtr<AEPBombBase>> BombList;

	/** 폭탄 오브젝트 풀의 최대 크기 */
	UPROPERTY(VisibleAnyWhere, Category = "Properties")
	int32 MaxBombCount;

	/** 현재 사용 가능한 폭탄의 개수 */
	UPROPERTY(VisibleAnyWhere, Category = "Properties")
	int32 CurrentBombCount;

protected:
	virtual void BeginPlay() override;

};
