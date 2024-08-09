// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "EPGameState.generated.h"

USTRUCT()
struct FScoreBoard
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<class AEPPlayerState> PlayerState;

	UPROPERTY()
	int32 Score = 0;

	bool operator==(const FScoreBoard& Other) const
	{
		return (PlayerState == Other.PlayerState) && (Score == Other.Score);
	}
};

UCLASS()
class EXPLOSION_API AEPGameState : public AGameState
{
	GENERATED_BODY()

public: 
	virtual void PostInitializeComponents() override;
	virtual double GetServerWorldTimeSeconds() const override;
	bool GetIsSetupEndMatch() const { return bIsSetupEndMatch; }
	void SetIsSetupEndMatch(bool bValue) { bIsSetupEndMatch = bValue; }
	float GetMatchTimeLimit() const { return MatchTimeLimit; }
	int32 GetMatchScoreLimit() const { return MatchScoreLimit; }

	void SetIsScoreBoardReplicated(bool bIsReplicated) { bIsScoreBoardReplicated = bIsReplicated; }
	bool GetIsScoreBoardReplicated() const { return bIsScoreBoardReplicated; }

	UFUNCTION()
	void OnRep_PostUpdateScoreBoard();

public:
	//UPROPERTY(Replicated)
	//TMap<TObjectPtr<APlayerState>, int32> ScoreBoard;

	UPROPERTY(ReplicatedUsing = OnRep_PostUpdateScoreBoard)
	TArray<FScoreBoard> ScoreBoard;

	void UpdateScoreBoard(TObjectPtr<class AEPPlayerState> KillerGameState);
	//void UpdateScoreBoard();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	int32 KillScoreMultiplier;

	UPROPERTY(Replicated)
	uint8 bIsSetupEndMatch : 1;

	int32 MatchScoreLimit;
	
	UPROPERTY(Replicated)
	float MatchTimeLimit;

private:
	uint8 bIsScoreBoardReplicated : 1;

};
