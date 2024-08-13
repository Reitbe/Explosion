// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EPPlayerState.generated.h"

/**
 * 본 게임에서 플레이어 개인의 정보를 저장하는 플레이어 스테이트.
 * Kill & Death를 저장하며 스코어보드 업데이트에 사용된다.
 */

UCLASS()
class EXPLOSION_API AEPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEPPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

// Replicate
public:
	/* KillCount가 업데이트 되면 로컬 컨트롤러의 스코어보드 업데이트*/
	UFUNCTION()
	void OnRep_UpdateKillCount();

	/* DeathCount가 업데이트 되면 로컬 컨트롤러의 스코어보드 업데이트*/
	UFUNCTION()
	void OnRep_UpdateDeathCount();

private:
	UPROPERTY(ReplicatedUsing = OnRep_UpdateKillCount)
	int32 KillCount;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateDeathCount)
	int32 DeathCount;

// Get & Set
public:	
	int32 GetKillCount() { return KillCount; };
	int32 GetDeathCount() { return DeathCount; };

	void AddKillCount() { ++KillCount; };
	void AddDeathCount() { ++DeathCount; };
};
