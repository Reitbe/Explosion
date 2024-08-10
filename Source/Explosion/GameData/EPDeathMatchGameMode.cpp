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

	//GameStateClass = AEPGameState::StaticClass();
	//PlayerControllerClass = AEPPlayerController::StaticClass();
	//PlayerStateClass = AEPPlayerState::StaticClass();
	//DefaultPawnClass = AEPCharacterPlayer::StaticClass();

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

	// 스폰포인트 액터 배열 초기화
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartPointClass, PlayerStartPointsArray);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ItemSpawnPointClass, ItemSpawnPointsArray);

	// 로비에 있던 전체 플레이어 수를 가져온다
}


FTransform AEPDeathMatchGameMode::GetRandomStartTransform()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartPointClass, PlayerStartPointsArray);
    if (PlayerStartPointsArray.Num() == 0)
    {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("이 씨발 어레이 못찾았구나.")));
        return FTransform(FVector(0.0f, 0.0f, 400.0f));
    }

	// 플레이어 스폰포인트 중에서 Free인 것을 찾아서 사용
	int32 RandomIndex = 0;
	APlayerStart* PlayerStartPoint = nullptr;

	do {
		RandomIndex = FMath::RandRange(0, PlayerStartPointsArray.Num() - 1);
		PlayerStartPoint = Cast<APlayerStart>(PlayerStartPointsArray[RandomIndex]);
	} while (PlayerStartPoint->PlayerStartTag != FName("Free"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("사용한 스폰포인트 : %d"), RandomIndex));

	// 사용한 스폰포인트는 5초 후 재사용 가능
	PlayerStartPoint->PlayerStartTag = FName("Occupied");
	OccupiedPlayerStartPointIndices.Enqueue(RandomIndex);

	FTimerHandle ReleasePlayerStartTagTimerHandle;
	GetWorldTimerManager().SetTimer(ReleasePlayerStartTagTimerHandle, FTimerDelegate::CreateLambda([&]() 
		{
			int32 ReleaseIndex = *OccupiedPlayerStartPointIndices.Peek();
			OccupiedPlayerStartPointIndices.Pop();
			Cast<APlayerStart>(PlayerStartPointsArray[ReleaseIndex])->PlayerStartTag = FName("Free");
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("플레이어 스폰포인트 태그 해제")));
		}
	), 3.0f, false);

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

	if (KillerPlayer != KilledPlayer) // 킬
	{
		KillerPlayerState->AddKillCount(); // 킬카운트 증가	
	}
	KilledPlayerState->AddDeathCount(); // 데스(자살 포함)
	EPGameState->UpdateScoreBoard(KillerPlayerState); // 스코어보드 업데이트
	// 스코어보드 UI 업데이트 - 서버만 호출, 클라이언트는 GameState의 OnRep_PostUpdateScoreBoard에서 호출
	GetGameState<AEPGameState>()->OnRep_PostUpdateScoreBoard();

	// 스코어 리미트 체크
	if (EPGameState->ScoreBoard.Num() > 0)
	{
		int32 HighestScore = EPGameState->ScoreBoard[0].Score;
		if (HighestScore >= EPGameState->GetMatchScoreLimit())
		{
			SetTheEndMatch();
		}
	}
}

void AEPDeathMatchGameMode::CheckAllPlayersReady()
{
	// 게임 인스턴스에서 값을 가져오지 못한 경우에는 종료 후 재호출
	++ReadyPlayerCount;

	PlayerCountInGame = GetLobbyPlayerCount();
	if (PlayerCountInGame == 0)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("준비된 플레이어 : %d, 전체 인원 : %d"), ReadyPlayerCount, PlayerCountInGame));
	if (ReadyPlayerCount >= PlayerCountInGame) 
	{
		// 맵 및 캐릭터 로딩시간 가진 이후 게임 시작
		FTimerHandle StartMainGameTimerHandle;
		GetWorldTimerManager().SetTimer(StartMainGameTimerHandle, this, &AEPDeathMatchGameMode::StartMainGame, 2.0f, false);
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

	// 모든플레이어 컨트롤러에 대하여 게임 종료 띄우기 호출
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

void AEPDeathMatchGameMode::StartMainGame()
{
	// 클라이언트 사이드에서 
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(PlayerController);
			if (EPPlayerController)
			{
				// 그냥 브로드캐스트? 아니면 클라RPC해서 거기서 브로드캐스트 
 				EPPlayerController->ClientRPC_StartMainGame();
			}
		}
	}

	// 스폰포인트 액터 배열 초기화
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerStartPointClass, PlayerStartPointsArray);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ItemSpawnPointClass, ItemSpawnPointsArray);

	// 서버에서도 인게임 타이머 시작
	FTimerHandle EndMatchTimerHandle;
	float EndMatchTime = GetGameState<AEPGameState>()->GetMatchTimeLimit();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("타이머 시작한다!")));

	GetWorldTimerManager().SetTimer(EndMatchTimerHandle, this, &AEPDeathMatchGameMode::SetTheEndMatch, EndMatchTime, false);
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

int32 AEPDeathMatchGameMode::GetLobbyPlayerCount() const
{
	int32 LobbyPlayerCount = 0;
	UEPGameInstance* EPGameInstance = Cast<UEPGameInstance>(GetGameInstance());

	// 게임 인스턴스가 존재한다면 플레이어 카운트를 가져오지만
	if (EPGameInstance)
	{
		LobbyPlayerCount = EPGameInstance->GetPlayerCount();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("플레이어 수 : %d"), PlayerCountInGame));
	}
	return LobbyPlayerCount;
}
