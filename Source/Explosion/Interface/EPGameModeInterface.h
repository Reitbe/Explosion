// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EPGameModeInterface.generated.h"

class AController;
class AActor;

UINTERFACE(MinimalAPI)
class UEPGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 게임모드가 구현해야 하는 필수 함수들을 정의한 인터페이스.
 * 실험적으로 2개의 기능만을 정의했다.
 */
class EXPLOSION_API IEPGameModeInterface
{
	GENERATED_BODY()

public:
	/* 플레이어가 부활할 장소를 랜덤하게 제공한다 */
	virtual FTransform GetRandomStartTransform() = 0;

	/* 플레이어 사망 시 처리 */
	virtual void OnPlayerKilled(AController* KillerPlayer, AController* KilledPlayer, AActor* DamageCauser) = 0;
};
