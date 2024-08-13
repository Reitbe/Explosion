// Fill out your copyright notice in the Description page of Project Settings.


#include "EPDeathMatchGameMode.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/GameData/EPGameInstance.h"
#include "Explosion/Player/EPPlayerController.h"
#include "Explosion/Character/EPCharacterPlayer.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


AEPDeathMatchGameMode::AEPDeathMatchGameMode()
{
	static ConstructorHelpers::FClassFinder<AEPGameState> GameStateClassFinder
	(TEXT("/Game/GameMode/BP_GameState.BP_GameState_C"));
	if (GameStateClassFinder.Class)
	{
		GameStateClass = GameStateClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AEPPlayerController> PlayerControllerClassFinder
	(TEXT("/Game/GameMode/BP_PlayerController.BP_PlayerController_C"));
	if (PlayerControllerClassFinder.Class)
	{
		PlayerControllerClass = PlayerControllerClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AEPPlayerState> PlayerStateClassFinder
	(TEXT("/Game/GameMode/BP_PlayerState.BP_PlayerState_C"));
	if (PlayerStateClassFinder.Class)
	{
		PlayerStateClass = PlayerStateClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AEPCharacterPlayer> DefaultPawnClassFinder
	(TEXT("/Game/Character/BP_CharacterPlayer.BP_CharacterPlayer_C"));
	if (DefaultPawnClassFinder.Class)
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

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

	ReturnToLobbyDelay = 5.0f;
	RespawnPointRecoverDelay = 3.0f;
}

void AEPDeathMatchGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEPDeathMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 스코어 보드에 새로운 플레이어 추가
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	if (EPGameState)
	{
		EPGameState->ScoreBoard.Add(FScoreBoard(Cast<AEPPlayerState>(NewPlayer->PlayerState), 0));
	}
}

void AEPDeathMatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	// 스코어 보드에 등록된 플레이어 제거
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	if (EPGameState)
	{	
		AEPPlayerState* ExitingPlayerState = Cast<AEPPlayerState>(Exiting->PlayerState);
		
		if (int32 idx = EPGameState->ScoreBoard.Find(FScoreBoard(ExitingPlayerState, 0)) != INDEX_NONE)
		{
			if (EPGameState->ScoreBoard.IsValidIndex(idx))
			{
				EPGameState->ScoreBoard.RemoveAt(idx);
			}
		}
	}
}

void AEPDeathMatchGameMode::StartPlay()
{
	Super::StartPlay();	
}


FTransform AEPDeathMatchGameMode::GetRandomStartTransform()
{
	// 레벨에 존재하는 스폰 포인트를 모두 가져온다.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartPointClass, PlayerStartPointsArray);
    
	// 스폰 포인트가 없거나 찾지 못한 경우에는 기본값을 사용.
	if (PlayerStartPointsArray.Num() == 0)
    {
        return FTransform(FVector(0.0f, 0.0f, 400.0f));
    }

	int32 RandomIndex = 0;
	APlayerStart* PlayerStartPoint = nullptr;

	// 사용 가능한 스폰 포인트를 찾을 때까지 탐색 진행
	do {
		RandomIndex = FMath::RandRange(0, PlayerStartPointsArray.Num() - 1);
		PlayerStartPoint = Cast<APlayerStart>(PlayerStartPointsArray[RandomIndex]);
	} while (PlayerStartPoint->PlayerStartTag != FName("Free"));

	// 탐색되어 사용할 스폰 포인트를 점유. 일정 시간 후 다시 해제하기 위하여 인덱스 저장.
	PlayerStartPoint->PlayerStartTag = FName("Occupied");
	OccupiedPlayerStartPointIndices.Enqueue(RandomIndex);

	FTimerHandle ReleasePlayerStartTagTimerHandle;
	GetWorldTimerManager().SetTimer(ReleasePlayerStartTagTimerHandle, FTimerDelegate::CreateLambda([&]() 
		{
			int32 ReleaseIndex = *OccupiedPlayerStartPointIndices.Peek();
			OccupiedPlayerStartPointIndices.Pop();
			Cast<APlayerStart>(PlayerStartPointsArray[ReleaseIndex])->PlayerStartTag = FName("Free");
		}
	), RespawnPointRecoverDelay, false);

    return PlayerStartPoint->GetActorTransform();
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

	// 킬이 발생한 경우 킬러의 킬카운트 증가. 자살인 경우는 제외한다.
	if (KillerPlayer != KilledPlayer) 
	{
		KillerPlayerState->AddKillCount(); 
	}
	// 데스카운트는 자살을 포함하여 공통으로 증가한다.
	KilledPlayerState->AddDeathCount(); 

	// 스코어보드의 내용을 업데이트한다. 
	EPGameState->UpdateScoreBoard(KillerPlayerState); 

	// 서버 플레이어의 스코어보드 UI 업데이트를 호출한다. 클라이언트는 GameState의 OnRep_PostUpdateScoreBoard에서 이를 호출한다. 
	GetGameState<AEPGameState>()->OnRep_PostUpdateScoreBoard();

	// 게임 종료 여부 확인
	if (CheckEndMatchCondition())
	{
		SetTheEndMatch();
	}
}

bool AEPDeathMatchGameMode::CheckEndMatchCondition()
{
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	if (EPGameState)
	{
		if (EPGameState->ScoreBoard.Num() > 0)
		{
			// 가장 점수가 높은 플레이어의 점수가 게임 종료 조건을 넘었는지 확인.
			int32 HighestScore = EPGameState->ScoreBoard[0].Score;
			if (HighestScore >= EPGameState->GetMatchScoreLimit())
			{
				return true;
			}
		}
	}
	return false;
}

void AEPDeathMatchGameMode::SetTheEndMatch()
{
	// 게임 스테이트에 매치가 종료중이라고 알리기
	AEPGameState* EPGameState = GetGameState<AEPGameState>();
	if (EPGameState)
	{
		EPGameState->SetIsSetupEndMatch(true);
	}

	// 별도로 모든플레이어 컨트롤러에 대하여 매치 종료를 전달
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

	// 결과 화면을 보여주기 위한 딜레이 타이머. 그 후 로비로 이동한다.
	FTimerHandle EndMatchTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(EndMatchTimerHandle, this, &AEPDeathMatchGameMode::EndMatch, ReturnToLobbyDelay, false);
}

void AEPDeathMatchGameMode::EndMatch()
{
	// 모든 플레이어의 로비 이동 및 세션 접속 해제 지시.
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

void AEPDeathMatchGameMode::CheckAllPlayersReady()
{
	// 각 유저가 준비 버튼을 눌렀을 때 호출되는 만큼 카운트 증가. 
	++ReadyPlayerCount;

	// 현재 게임에 참여중인 플레이어 수를 가져온다.
	PlayerCountInGame = GetLobbyPlayerCount();

	/*
	* 값을 가져오지 못한 경우에는 게임이 시작되지 않는다.
	* 첫 플레이어가 트래블 이후 바로 게임 인스턴스의 값을 가져오는 경우, 아예 값이 지정되지 않는 경우에 해당된다.
	* 전자라면 두 번째 플레이어가 접속하여 본 함수를 호출했을 때 값을 가져올 수 있으므로, 게임을 시작할 수 있다. 게임 플레이 인원은 최소 2명이다.
	*/
	if (PlayerCountInGame == 0)
	{
		return;
	}

	// 모든 플레이어가 플레이 할 준비가 되었을 때 게임을 시작한다.
	if (ReadyPlayerCount >= PlayerCountInGame) 
	{
		FTimerHandle StartMainGameTimerHandle;
		GetWorldTimerManager().SetTimer(StartMainGameTimerHandle, this, &AEPDeathMatchGameMode::StartMainGame, 2.0f, false);
	}
}

int32 AEPDeathMatchGameMode::GetLobbyPlayerCount() const
{
	int32 LobbyPlayerCount = 0;
	UEPGameInstance* EPGameInstance = Cast<UEPGameInstance>(GetGameInstance());

	// 게임 인스턴스에서 '현재 게임에 참여중인 플레이어 수'를 가져온다. 로비에서 준비를 누른 인원 수와 동일하다.
	if (EPGameInstance)
	{
		LobbyPlayerCount = EPGameInstance->GetPlayerCount();
	}
	return LobbyPlayerCount;
}

void AEPDeathMatchGameMode::StartMainGame()
{
	// 각 클라이언트에게 모든 플레이어가 준비되어 게임이 시작됨을 알린다.
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(PlayerController);
			if (EPPlayerController)
			{
 				EPPlayerController->ClientRPCStartMainGame();
			}
		}
	}

	// 스폰포인트 액터 배열 초기화
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartPointClass, PlayerStartPointsArray);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ItemSpawnPointClass, ItemSpawnPointsArray);

	// 매치 종료를 위한 타이머를 시작한다.
	FTimerHandle EndMatchTimerHandle;
	float EndMatchTime = GetGameState<AEPGameState>()->GetMatchTimeLimit();
	GetWorldTimerManager().SetTimer(EndMatchTimerHandle, this, &AEPDeathMatchGameMode::SetTheEndMatch, EndMatchTime, false);
}



