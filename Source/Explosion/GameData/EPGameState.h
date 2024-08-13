// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "EPGameState.generated.h"

class AEPPlayerState;

/**
 * 전체 플레이어의 점수 관리에 사용되는 구조체.
 */

USTRUCT()
struct FScoreBoard
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AEPPlayerState> PlayerState;

	UPROPERTY()
	int32 Score = 0;

	bool operator==(const FScoreBoard& Other) const
	{
		return (PlayerState == Other.PlayerState) && (Score == Other.Score);
	}
};

/*
* 서버와 클라이언트가 공통으로 사용하는 게임 상태 정보를 관리하는 클래스.
* 플레이어별 점수를 나타내는 스코어보드와 매치 진행 관련 정보를 관리한다.
*/

UCLASS()
class EXPLOSION_API AEPGameState : public AGameState
{
	GENERATED_BODY()

public: 
	virtual void PostInitializeComponents() override;

	/* 서버 시간을 반환하는 함수. 서버와 클라이언트에서 다르게 동작.  */
	virtual double GetServerWorldTimeSeconds() const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


// 스코어보드 Replicate
public:
	/* 플레이어 킬이 발생할 시 해당 유저의 점수를 업데이트하고 전체 점수 순위를 재정렬 */
	void UpdateScoreBoard(TObjectPtr<AEPPlayerState> KillerGameState);

	/* 변경된 스코어보드 정보를 각 클라이언트의 UI에 반영하라 전달 */
	UFUNCTION()
	void OnRep_PostUpdateScoreBoard();

	/* 
	* 각 플레이어의 상태와 점수를 저장하고 있는 배열 
	* TMap이 아닌 구조체 TArray인 이유는 Replicate 때문이다.
	*/
	UPROPERTY(ReplicatedUsing = OnRep_PostUpdateScoreBoard)
	TArray<FScoreBoard> ScoreBoard;


// Get & Set
public:
	bool GetIsSetupEndMatch() const { return bIsSetupEndMatch; }
	void SetIsSetupEndMatch(bool bValue) { bIsSetupEndMatch = bValue; }

	float GetMatchTimeLimit() const { return MatchTimeLimit; }
	int32 GetMatchScoreLimit() const { return MatchScoreLimit; }



protected:
	/* 1킬당 점수 가중치 */
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	int32 KillScoreMultiplier;

	/* 매치가 종료되었는지 여부 */
	UPROPERTY(Replicated)
	uint8 bIsSetupEndMatch : 1;

	/* 매치가 진행될 시간 */
	UPROPERTY(Replicated)
	float MatchTimeLimit;

	/* 매치 종료 기준 점수 */
	int32 MatchScoreLimit;

};
