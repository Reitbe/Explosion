// Fill out your copyright notice in the Description page of Project Settings.


#include "EPDeathMatchGameMode.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/Player/EPPlayerController.h"
#include "Explosion/Character/EPCharacterPlayer.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AEPDeathMatchGameMode::AEPDeathMatchGameMode()
{
	GameStateClass = AEPGameState::StaticClass();
	PlayerControllerClass = AEPPlayerController::StaticClass();
	PlayerStateClass = AEPPlayerState::StaticClass();
	DefaultPawnClass = AEPCharacterPlayer::StaticClass();

	static ConstructorHelpers::FClassFinder<APlayerStart> PlayerStartPointClassFinder
	(TEXT("/Game/Map/BP_PlayerSpawnPoint.BP_PlayerSpawnPoint_C"));
	if (PlayerStartPointClassFinder.Class)
	{
		PlayerStartPointClass = PlayerStartPointClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerStart> ItemSpawnPointClassFinder
	(TEXT("/Game/Map/BP_ItemSpawnPoint.BP_ItemSpawnPoint_C"));
	if (ItemSpawnPointClassFinder.Class)
	{
		ItemSpawnPointClass = ItemSpawnPointClassFinder.Class;
	}

	MatchScoreLimit = 30;
	//MatchTimeLimit = 300.0f;
	MatchTimeLimit = 6.0f; // 게임 스테이트에서 지정할 것.

	ReturnToLobbyDelay = 10.0f;
}

void AEPDeathMatchGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEPDeathMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	if (EPGameState)
	{
		EPGameState->ScoreBoard.Add(FScoreBoard(Cast<AEPPlayerState>(NewPlayer->PlayerState), 0));
	}
}

void AEPDeathMatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	if (EPGameState)
	{	
		AEPPlayerState* ExitingPlayerState = Cast<AEPPlayerState>(Exiting->PlayerState);
		
		if (int32 idx = EPGameState->ScoreBoard.Find(FScoreBoard(ExitingPlayerState, 0)) != INDEX_NONE)
		{
			if (EPGameState->ScoreBoard.IsValidIndex(idx))
			{
				FString PName = EPGameState->ScoreBoard[idx].PlayerState->GetPlayerName();
				EPGameState->ScoreBoard.RemoveAt(idx);
			}
		}
	}
}

void AEPDeathMatchGameMode::StartPlay()
{
	Super::StartPlay();	
	FTimerHandle EndMatchTimerHandle;
	float EndMatchTime = GetGameState<AEPGameState>()->GetMatchTimeLimit();
	GetWorldTimerManager().SetTimer(EndMatchTimerHandle, this, &AEPDeathMatchGameMode::SetTheEndMatch, EndMatchTime, false);

	// 스폰포인트 액터 배열 초기화
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartPointClass, PlayerStartPointsArray);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ItemSpawnPointClass, ItemSpawnPointsArray);
}


FTransform AEPDeathMatchGameMode::GetRandomStartTransform()
{
    if (PlayerStartPointsArray.Num() == 0)
    {
        return FTransform(FVector(0.0f, 0.0f, 400.0f));
    }

    int32 RandomIndex = FMath::RandRange(0, PlayerStartPointsArray.Num() - 1);
    return PlayerStartPointsArray[RandomIndex]->GetActorTransform();
}

FTransform AEPDeathMatchGameMode::GetRandomItemSpawnTransform()
{
	// 임시 기능으로 랜덤 포인트에 아이템을 배치한 상태로 시작
	if (ItemSpawnPointsArray.Num() == 0)
	{
		return FTransform(FVector(0.0f, 0.0f, 0.0f));
	}

	int32 RandomIndex = FMath::RandRange(0, ItemSpawnPointsArray.Num() - 1);
	return ItemSpawnPointsArray[RandomIndex]->GetActorTransform();
}

void AEPDeathMatchGameMode::OnPlayerKilled(AController* KillerPlayer, AController* KilledPlayer, AActor* DamageCauser)
{
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	AEPPlayerState* KillerPlayerState = KillerPlayer->GetPlayerState<AEPPlayerState>();
	AEPPlayerState* KilledPlayerState = KilledPlayer->GetPlayerState<AEPPlayerState>();

	if(!EPGameState || !KillerPlayerState || !KilledPlayerState)
	{
		return;
	}

	if (KillerPlayer != KilledPlayer) // 킬
	{
		KillerPlayerState->AddKillCount(); // 킬카운트 증가	
	}
	KilledPlayerState->AddDeathCount(); // 데스(자살 포함)
	EPGameState->UpdateScoreBoard(KillerPlayerState); // 스코어보드 업데이트

	// 스코어 리미트 체크
	if (EPGameState->ScoreBoard.Num() > 0)
	{
		int32 HighestScore = EPGameState->ScoreBoard[0].Score;
		if (HighestScore >= EPGameState->GetMatchScoreLimit())
		{
			SetTheEndMatch();
		}
	}

	// 개별 스코어 보드 업데이트
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(PlayerController);
			if (EPPlayerController)
			{
				EPPlayerController->ClientRPCUpdateScoreBoard();
			}
		}
	}
}

void AEPDeathMatchGameMode::SetTheEndMatch()
{
	// 게임 스테이트에 매치가 종료중이라고 알리기
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	if (EPGameState)
	{
		EPGameState->SetIsSetupEndMatch(true);
	}

	// 모든플레이어 컨트롤러에 대하여 사망 띄우기 호출
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(PlayerController);
			if (EPPlayerController)
			{
				EPPlayerController->ClientRPCSetupEndMatch();
			}
		}
	}

	FTimerHandle EndMatchTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EndMatchTimerHandle, this, &AEPDeathMatchGameMode::EndMatch, ReturnToLobbyDelay, false);
}

void AEPDeathMatchGameMode::EndMatch()
{
	// 접속중인 모든클라에 대하여 종료하기. 그리고 로비로 이동.
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(PlayerController);
			if (EPPlayerController)
			{
				EPPlayerController->ClientRPCEndMatch();
			}
		}
	}
}
