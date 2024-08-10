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
	SetIsScoreBoardReplicated(true);

	AEPPlayerController* PlayerController = Cast<AEPPlayerController>(GetWorld()->GetFirstPlayerController());
	//PlayerController->ClientRPCUpdateScoreBoard();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ScoreBoard Updated"));
	PlayerController->UpdateScoreBoard();
}


void AEPGameState::UpdateScoreBoard(TObjectPtr<AEPPlayerState> KillerGameState)
{
	// ScoreBoard에서 가진 플레이어 스테이트가 이거랑 맞는지 확인 -> 맞으면 업데이트.
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

