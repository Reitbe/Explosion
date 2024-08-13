// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EPLobbyPlayerState.generated.h"

/**
 * 로비에서 사용되는 플레이어 스테이트. 준비 여부를 저장한다. 
 */

UCLASS()
class EXPLOSION_API AEPLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AEPLobbyPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool GetIsReady() const { return bIsReady; }
	void SetIsReady(bool bReady) { bIsReady = bReady; }

private:
	/*
	* 플레이어의 준비 상태를 나타낸다.
	* OnRep 함수가 없는 이유는 게임모드에서 각 플레이어 스테이트를 순회하며 준비 여부를 확인하기 때문.
	* 특정 플레이어의 참가와 퇴장 여부에 관련 없이 오로지 준비된 플레이어 총원을 사용하기 위함이다.
	*/
	UPROPERTY(Replicated)
	uint8 bIsReady : 1;
	
};
