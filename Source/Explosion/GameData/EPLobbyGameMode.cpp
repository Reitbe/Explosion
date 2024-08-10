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

	//static ConstructorHelpers::FClassFinder<AEPLobbyStatue> LobbyStatueClassFinder
	//(TEXT("/Game/Map/BP_LobbyStatueWithPedestal.BP_LobbyStatueWithPedestal_C"));
	//if (LobbyStatueClassFinder.Class)
	//{
	//	LobbyStatueWithPedestalsClass = LobbyStatueClassFinder.Class;
	//}

	//MainGameMapName = "/Game/Map/TravelTest.TravelTest?listen";
	MainGameMapName = "BattleRoyalMap?listen";

	ReadyPlayerCount = 0;
	MaxPlayers = 0;
	bIsFirstLoginCompleted = false;

	bUseSeamlessTravel = false;
}

void AEPLobbyGameMode::StartPlay()
{
	Super::StartPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("스타트 플레이 시작")));
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), LobbyStatueWithPedestalsClass, LobbyStatueWithPedestalsArray);

	bIsFirstLoginCompleted = true;
	MultiplayerSessionSubsystem = GetGameInstance()->GetSubsystem<UEPMultiplayerSessionSubsystem>();
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &AEPLobbyGameMode::OnCreateSessionComplete);
		MultiplayerSessionSubsystem->MultiPlayerOnSessionParticipantJoined.AddDynamic(this, &AEPLobbyGameMode::OnSessionParticipantJoined);
		MultiplayerSessionSubsystem->MultiPlayerOnSessionParticipantLeft.AddDynamic(this, &AEPLobbyGameMode::OnSessionParticipantLeft);

		// 모든 석상이 로드된 이후 최초의 석상 업데이트
		UpdateLobbyStatue();
	}

}

void AEPLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 첫 로그인(본인)에 석상이 로드되지 않았으므로 업데이트를 하지 않음
	if (bIsFirstLoginCompleted) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PostLogin에서 로비 석상 업데이트 호출")));
		UpdateLobbyStatue();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PostLogin이지만 세션 안맏르어져서 호출 안함.")));
	}
}

void AEPLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Logout!!")));

	if (bIsFirstLoginCompleted)
	{
		UpdateLobbyStatue();
	}
}

void AEPLobbyGameMode::OnCreateSessionComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("게임모드 세션 컴플맅트 실행.")));
	if (MultiplayerSessionSubsystem)
	{
		MaxPlayers = 0;
		ReadyPlayerCount = 0;
		//bIsFirstLoginCompleted = true;
	}
}


void AEPLobbyGameMode::OnSessionParticipantJoined()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("새로운 멤버가 들어왔습니다.")));
	//UpdateLobbyStatue();
}

void AEPLobbyGameMode::OnSessionParticipantLeft()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("멤버가 나갔습니다.")));
	//UpdateLobbyStatue();
}


void AEPLobbyGameMode::UpdatePlayerCount()
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

	// 세션 참가 가능한 최대 인원 수 확인
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

	// 플레이어 수 확인 후 시작
	if (ReadyPlayerCount >= MaxPlayers) // 임의로 1로 설정. 혼자서 레디하면 게임 하게끔. 
	{
		FTimerHandle StartMainGameTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(StartMainGameTimerHandle, this, &AEPLobbyGameMode::StartMainGame, 2.0f, false);
	}
}



void AEPLobbyGameMode::UpdateLobbyStatue()
{
	bool bIsOnMainMenu = false;
	FString SessionState = MultiplayerSessionSubsystem->GetSessionState();
	
	// 메인화면(NoSession)인 경우 석상 전체 비활성화
	bIsOnMainMenu = (SessionState != FString(TEXT("InProgress")) && SessionState != FString(TEXT("Pending"))) ? true : false;
	

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

// 서버트래블이 어디서 안되는지 테스트용
bool AEPLobbyGameMode::CanServerTravel(const FString& FURL, bool bAbsolute)
{
	UWorld* World = GetWorld();

	check(World);

	if (FURL.Contains(TEXT("%")))
	{
		UE_LOG(LogGameMode, Error, TEXT("CanServerTravel: FURL %s Contains illegal character '%%'."), *FURL);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("URL에 '%%'들어가서 실패")));
		return false;
	}

	if (FURL.Contains(TEXT(":")) || FURL.Contains(TEXT("\\")))
	{
		UE_LOG(LogGameMode, Error, TEXT("CanServerTravel: FURL %s blocked, contains : or \\"), *FURL);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("URL에 ':' or '\\'들어가서 실패")));
		return false;
	}

	FString MapName;
	int32 OptionStart = FURL.Find(TEXT("?"));
	if (OptionStart == INDEX_NONE)
	{
		MapName = FURL;
	}
	else
	{
		MapName = FURL.Left(OptionStart);
	}

	// Check for invalid package names.
	FText InvalidPackageError;
	if (MapName.StartsWith(TEXT("/")) && !FPackageName::IsValidLongPackageName(MapName, true, &InvalidPackageError))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("CanServerTravel: FURL %s blocked (%s)"), *FURL, *InvalidPackageError.ToString()));
		return false;
	}

	return true;
}

void AEPLobbyGameMode::StartMainGame()
{
	// 현재 게임 참가자 수를 다음 게임모드에 전달하기 위한 저장
	UEPGameInstance* EPGameInstance = Cast<UEPGameInstance>(GetGameInstance());
	if (EPGameInstance)
	{
		EPGameInstance->SetPlayerCount(MaxPlayers);

		int32 Temp = EPGameInstance->GetPlayerCount();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("플레이어 수 : %d"), Temp));
	}

	

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("서버트래블 시도합니다.")));

	if (CanServerTravel(MainGameMapName, true))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("자체 조사 성공")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("자체조사 실패")));
	}

	if (GetWorld()->ServerTravel(MainGameMapName, true))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("서버트래블 성공!!")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("서버트래블 실패!!")));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("서버트래블 이후")));
}
