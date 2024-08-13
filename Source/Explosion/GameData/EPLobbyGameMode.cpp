// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyGameMode.h"
#include "Explosion/Player/EPLobbyPlayerController.h"
#include "Explosion/Character/EPLobbyCharacter.h"
#include "Explosion/Prop/EPLobbyStatue.h"
#include "Explosion/GameData/EPLobbyPlayerState.h"
#include "Explosion/GameData/EPMultiplayerSessionSubsystem.h"
#include "Explosion/GameData/EPGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

AEPLobbyGameMode::AEPLobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<AEPLobbyPlayerController> PlayerControllerClassFinder
	(TEXT("/Game/GameMode/BP_LobbyPlayerController.BP_LobbyPlayerController_C"));
	if (PlayerControllerClassFinder.Class)
	{
		PlayerControllerClass = PlayerControllerClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AEPLobbyCharacter> DefaultPawnClassFinder
	(TEXT("/Game/Character/BP_LobbyPlayer.BP_LobbyPlayer_C"));
	if (DefaultPawnClassFinder.Class)
	{
		DefaultPawnClass = DefaultPawnClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<AEPLobbyPlayerState> PlayerStateClassFinder
	(TEXT("/Game/GameMode/BP_LobbyPlayerState.BP_LobbyPlayerState_C"));
	if (PlayerStateClassFinder.Class)
	{
		PlayerStateClass = PlayerStateClassFinder.Class;
	}

	MainGameMapName = "BattleRoyalMap?listen";

	ReadyPlayerCount = 0;
	MaxPlayers = 0;
	bIsFirstLoginCompleted = false;

	// 심리스 트래블을 사용하지 않는다!! 게임모드부터 모든 것을 새로 생성하기 위함이다. 
	bUseSeamlessTravel = false;
}

void AEPLobbyGameMode::StartPlay()
{
	Super::StartPlay();

	// 서버 유저의 PostLogin은 이미 실행되었으므로 이후의 로그인에서 석상 업데이트를 허용하기 위함. 
	bIsFirstLoginCompleted = true;

	MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();

	// 모든 석상이 로드된 이후 최초의 석상 업데이트
	UpdateLobbyStatue();
}

void AEPLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 서버 본인의 첫 로그인 시점에는 석상이 로드되지 않았으므로 석상 업데이트를 하지 않는다.
	if (bIsFirstLoginCompleted) 
	{
		UpdateLobbyStatue();
	}
}

void AEPLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	if (bIsFirstLoginCompleted)
	{
		UpdateLobbyStatue();
	}
}

void AEPLobbyGameMode::UpdateReadyPlayerCount()
{
	ReadyPlayerCount = 0;

	// 전체 인원에 대하여 레디 상태 확인
	auto& PlayerArray = GetGameState<AGameState>()->PlayerArray;
	for(int32 i = 0; i<PlayerArray.Num(); ++i)
	{
		AEPLobbyPlayerState* PlayerState = Cast<AEPLobbyPlayerState>(PlayerArray[i]);
		if (PlayerState)
		{
			if (PlayerState->GetIsReady())
			{
				++ReadyPlayerCount;
			}
		}
	}

	// 세션에서 함께 플레이할 수 있는 최대 인원 수 확인
	if (MultiplayerSessionSubsystem)
	{
		FNamedOnlineSession* Session = MultiplayerSessionSubsystem->GetSession();
		MaxPlayers = Session->SessionSettings.NumPublicConnections;
	}

	// 각 플레이어에게 최대 연결 가능 수 대비 현 인원 수 전달
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AEPLobbyPlayerController* EPLobbyPlayerController = Cast<AEPLobbyPlayerController>(PlayerController);
			if (EPLobbyPlayerController)
			{
				EPLobbyPlayerController->ClientRPC_UpdatePalyerCount(ReadyPlayerCount, MaxPlayers);
			}
		}
	}

	CheckStartMainGame();
}

void AEPLobbyGameMode::CheckStartMainGame()
{
	// 모든 플레이어가 준비되었을 때, 약간의 유예를 두고 메인 게임을 시작한다.
	if (ReadyPlayerCount >= MaxPlayers)
	{
		FTimerHandle StartMainGameTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(StartMainGameTimerHandle, this, &AEPLobbyGameMode::StartMainGame, 1.0f, false);
	}
}


void AEPLobbyGameMode::UpdateLobbyStatue()
{
	/*
	* 세션 스테이트를 기반으로 메인화면과 로비 화면을 구분한다.
	* 세션이 진행중인 경우 InProgress(서버) or Pending(클라이언트) 상태이며
	* 세션이 진행중이 아닌 경우 NoSession 상태이다.
	*/
	FString SessionState = MultiplayerSessionSubsystem->GetSessionState();
	bool bIsOnMainMenu = (SessionState != FString(TEXT("InProgress")) && SessionState != FString(TEXT("Pending"))) ? true : false;
	

	// 전체 플레이어에 대하여 석상 활성화
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController)
		{
			AEPLobbyPlayerController* EPLobbyPlayerController = Cast<AEPLobbyPlayerController>(PlayerController);
			if (EPLobbyPlayerController)
			{
				EPLobbyPlayerController->ClientRPC_UpdateLobbyStatue(bIsOnMainMenu);
			}
		}
	}
}

void AEPLobbyGameMode::StartMainGame()
{
	// 현재 로비에 참가한 총 플레이어 수를 저장하여 다음 레벨에서 사용할 수 있게끔 저장한다.
	UEPGameInstance* EPGameInstance = Cast<UEPGameInstance>(GetGameInstance());
	if (EPGameInstance)
	{
		EPGameInstance->SetPlayerCount(MaxPlayers);
	}

	// 서버트래블
	GetWorld()->ServerTravel(MainGameMapName, true);
}
