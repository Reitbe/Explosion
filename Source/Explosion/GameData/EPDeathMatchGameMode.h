// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Explosion/Interface/EPGameModeInterface.h"
#include "EPDeathMatchGameMode.generated.h"

class APlayerStart;

/**
 * 메인 게임모드. 
 * 데스매치 형식이며 개인전으로 진행된다. 
 */

UCLASS()
class EXPLOSION_API AEPDeathMatchGameMode : public AGameMode, public IEPGameModeInterface
{
	GENERATED_BODY()

public:
	AEPDeathMatchGameMode();

// IEPGameModeInterface
public:
	/* 플레이어 리스폰 시 보유중인 플레이어 스타트 중 한 곳의 정보를 랜덤하게 제공 */
	virtual FTransform GetRandomStartTransform() override;

	/* 플레이어 사망 시 플레이어 상태 업데이트 및 게임 종료 조건 확인*/
	virtual void OnPlayerKilled(AController* KillerPlayer, AController* KilledPlayer, AActor* DamageCauser) override;


// 게임 핵심 로직
public:
	/* 레벨에 배치된 아이템 스폰 위치 중 랜덤한 값을 반환 */
	FTransform GetRandomItemSpawnTransform();

public:
	/* 모든 플레이어가 메인 게임에 접속하여 플레이 할 준비가 되었는지 확인 */
	void CheckAllPlayersReady();

protected:
	/* 모든 플레이어가 준비 되었을 때 메인 게임을 시작 */
	void StartMainGame();
	
	/* 킬이 발생했을 때 매치 종료 조건을 확인 */
	bool CheckEndMatchCondition();

	/* 매치 종료 조건이 충족되었을 때 매치 종료 처리. 매치가 종료되고나서도 잠시 해당 레벨에 남아 점수판을 표시한다.  */
	void SetTheEndMatch();
	
	/* 매치의 완전한 종료. 로비 레벨로 이동한다. */
	void EndMatch();

	int32 GetLobbyPlayerCount() const;


// 플레이어 및 아이템 스폰
protected:
	TSubclassOf<APlayerStart> PlayerStartPointClass;
	TSubclassOf<APlayerStart> ItemSpawnPointClass;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> PlayerStartPointsArray;

	/* 사용한 플레이어 스폰 위치를 재사용 할 수 있도록 태그 복구 대기중인 목록 */
	TQueue<int32> OccupiedPlayerStartPointIndices;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AActor>> ItemSpawnPointsArray;

protected:
	/* 매치가 종료되고나서 로비 레벨로 이동할 때 까지의 대기 시간*/
	int32 ReturnToLobbyDelay;

	/* 사용한 플레이어 스폰 위치를 다시 사용할 수 있게 될 때 까지의 대기 시간 */
	int32 RespawnPointRecoverDelay;

private:
	/* 레벨 이동 후 게임을 시작할 준비가 된 플레이어의 수 */
	int32 ReadyPlayerCount{ 0 };

	/* 현재 세션에 참여한 플레이어 수. 로딩된 수가 아니라 로비에서 준비버튼을 누른 총 수이다. */
	int32 PlayerCountInGame{ 0 }; 


public:
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void StartPlay() override;

};
