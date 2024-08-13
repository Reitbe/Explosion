// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EPLobbyGameMode.generated.h"

class UEPMultiplayerSessionSubsystem;

/**
 * 메인 메뉴와 로비에서 사용되는 게임 모드이다.
 * 석상 업데이트와 플레이어 준비 상태에 따른 메인 게임 시작을 관리한다.
 * 석상은 참가 인원이 변동될 때, 레디 플레이어 카운트는 준비 인원이 변동될 때 업데이트 된다. 
 */

UCLASS()
class EXPLOSION_API AEPLobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AEPLobbyGameMode();

	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

public:
	/* 준비 버튼을 누른 인원에 변동이 있을 때, 각 클라이언트의 UI 업데이트를 호출 */
	void UpdateReadyPlayerCount();

protected:
	/* 로비 참여 인원에 변동이 있을 때, 각 클라이언트의 석상 활성화 업데이트를 호출 */
	void UpdateLobbyStatue();

	/* 모든 플레이어가 준비되었는지 여부를 확인 */
	void CheckStartMainGame();

	/* 메인 게임을 시작하기 위해 서버 트래블 진행 */
	void StartMainGame();

protected:
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

private:
	/* 준비를 마친 총 인원의 수 */
	int32 ReadyPlayerCount;

	/* 세션에 참가 가능한 최대 플레이어 수 */
	int32 MaxPlayers;

	/* 서버 유저의 첫 로그인 완료 여부 */
	uint8 bIsFirstLoginCompleted : 1;

	/* 메인 게임 레벨로의 이동에 필요한 경로 */
	FString MainGameMapName;
};
