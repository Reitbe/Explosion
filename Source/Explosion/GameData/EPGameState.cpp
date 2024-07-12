// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameState.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "Explosion/Player/EPPlayerController.h"
#include "Explosion/GameData/EPDeathMatchGameMode.h"
#include "Net/UnrealNetwork.h"

void AEPGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	KillScoreMultiplier = 10;
	bIsSetupEndMatch = false;

	// 게임모드에 있어야할 종료 조건이지만 임시로 여기 두다.->타이머 UI를 위해
	// 이거 게임모드에서 얻어와서 
	MatchScoreLimit = 30;
	MatchTimeLimit = 300.0f;
}

double AEPGameState::GetServerWorldTimeSeconds() const
{
	
	if (AEPPlayerController* PC = Cast<AEPPlayerController>(GetGameInstance()->GetFirstLocalPlayerController()))
	{
		return PC->GetServerTime();
	}
	else
	{
		return GetWorld()->GetTimeSeconds();
	}
	return 0.0;
}

void AEPGameState::UpdateScoreBoard()
{
	for (auto& Player : PlayerArray)
	{
		AEPPlayerState* EPPlayerState = Cast<AEPPlayerState>(Player);
		if (EPPlayerState)
		{
			int32 KillCount = EPPlayerState->GetKillCount();
			if (ScoreBoard.Contains(EPPlayerState))
			{
				ScoreBoard[EPPlayerState] = KillCount * KillScoreMultiplier;
			}
			else
			{
				ScoreBoard.Add(EPPlayerState, KillCount * KillScoreMultiplier);
			}
		}
	}
}

void AEPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//DOREPLIFETIME(AEPGameState, ScoreBoard);
	DOREPLIFETIME(AEPGameState, bIsSetupEndMatch);
}


void AEPGameState::UpdateScoreBoard(TObjectPtr<AEPPlayerState> KillerGameState)
{
	if(ScoreBoard.Contains(KillerGameState))
	{
		int32 KillCount = KillerGameState->GetKillCount();
		ScoreBoard[KillerGameState] = KillCount * KillScoreMultiplier;
	}
	ScoreBoard.ValueSort([](int32 A, int32 B) { return A > B; });
}

