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

	MatchScoreLimit = 30;
	MatchTimeLimit = 120.0f;
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

void AEPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEPGameState, ScoreBoard);
	DOREPLIFETIME(AEPGameState, bIsSetupEndMatch);
	DOREPLIFETIME(AEPGameState, MatchTimeLimit);
}

void AEPGameState::OnRep_PostUpdateScoreBoard()
{
	AEPPlayerController* PlayerController = Cast<AEPPlayerController>(GetWorld()->GetFirstPlayerController());
	PlayerController->UpdateScoreBoard();
}


void AEPGameState::UpdateScoreBoard(TObjectPtr<AEPPlayerState> KillerGameState)
{
	// 점수 변경이 필요한 플레이어를 찾아서 점수를 업데이트하고 점수가 높은 순서대로 정렬한다. 
	for (auto& PlyserScoreStruct : ScoreBoard)
	{
		if (PlyserScoreStruct.PlayerState == KillerGameState)
		{
			int32 KillCount = KillerGameState->GetKillCount();
			PlyserScoreStruct.Score = KillCount * KillScoreMultiplier;
			break;
		}
	}	
	ScoreBoard.Sort([](FScoreBoard A, FScoreBoard B) { return A.Score > B.Score; });
}

