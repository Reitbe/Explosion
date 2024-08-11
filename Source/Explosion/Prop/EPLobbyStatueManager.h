// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPLobbyStatueManager.generated.h"

/**
 * 로비 레벨에 등장하는 플레이어 석상을 관리는 매니저.
 * 세션에 플레이어가 들어오거나 나갈 때 석상의 수와 이름표를 업데이트한다.
*/

UCLASS()
class EXPLOSION_API UEPLobbyStatueManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEPLobbyStatueManager();

	/* 레벨에 존재하는 모든 석상들을 수집한다 */
	void GetAllLobbyStatue();

	/* 활성화 된 석상의 수와 이름표를 업데이트한다. 메인메뉴의 경우 모든 석상을 비활성화한다. */
	void UpdateLobbyStatue(bool IsOnMainMenu);

protected:
	/* 수집하고자 하는 석상 클래스 */
	TSubclassOf<class AEPLobbyStatue> LobbyStatueWithPedestalsClass;

	/* 수집한 석상들의 목록*/
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> LobbyStatueWithPedestalsArray;

	/* 플레이어 이름 목록에 접근하기 위하여 플레이어 스테이트가 준비될 때 까지 기다리는 타이머 핸들*/
	FTimerHandle GameStateIsReadyTimerHandle;
};
