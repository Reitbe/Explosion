// Fill out your copyright notice in the Description page of Project Settings.


#include "EPPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Explosion/UI/EPGameMenuWidget.h"
#include "Explosion/UI/EPScoreBoardWidget.h"
#include "Explosion/UI/EPGameEndWidget.h"
#include "Explosion/UI/EPBasicTextWidget.h"
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "Components/Button.h"



AEPPlayerController::AEPPlayerController()
{

// 입력
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextFinder
	(TEXT("/Game/Input/IMC_PlayerController.IMC_PlayerController"));
	if (InputMappingContextFinder.Succeeded())
	{
		InputMappingContext = InputMappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ScoreBoardInputActionFinder
	(TEXT("/Game/Input/IA_ShowScoreBoard.IA_ShowScoreBoard"));
	if (ScoreBoardInputActionFinder.Succeeded())
	{
		ShowingScoreBoard = ScoreBoardInputActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> GameMenuInputActionFinder
	(TEXT("/Game/Input/IA_ShowGameMenu.IA_ShowGameMenu"));
	if (GameMenuInputActionFinder.Succeeded())
	{
		ShowingGameMenu = GameMenuInputActionFinder.Object;
	}
	
// 위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> MatchEndWidgetClassFinder
	(TEXT("/Game/UI/WBP_MatchEndWidget.WBP_MatchEndWidget_C"));
	if (MatchEndWidgetClassFinder.Class)
	{
		MatchEndWidgetClass = MatchEndWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UEPGameMenuWidget> GameMenuWidgetClassFinder
	(TEXT("/Game/UI/WBP_GameMenuWidget.WBP_GameMenuWidget_C"));
	if (GameMenuWidgetClassFinder.Class)
	{
		GameMenuWidgetClass = GameMenuWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UEPScoreBoardWidget> ScoreBoardWidgetClassFinder
	(TEXT("/Game/UI/WBP_ScoreBoardWidget.WBP_ScoreBoardWidget_C"));
	if (ScoreBoardWidgetClassFinder.Class)
	{
		ScoreBoardWidgetClass = ScoreBoardWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UEPBasicTextWidget> StartMainGameWidgetClassFinder
	(TEXT("/Game/UI/WBP_MainGameStartWidget.WBP_MainGameStartWidget_C"));
	if (StartMainGameWidgetClassFinder.Class)
	{
		StartMainGameWidgetClass = StartMainGameWidgetClassFinder.Class;
	}

	ServerTime = 0.0f;
}

void AEPPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AEPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 입력 컨텍스트 등록
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr) {
		Subsystem->AddMappingContext(InputMappingContext, 0); // 캐릭터는 Priority가 1이다
	}

	if (IsLocalController())
	{
		if (ScoreBoardWidgetClass)
		{
			ScoreBoardWidget = CreateWidget<UEPScoreBoardWidget>(this, ScoreBoardWidgetClass);
			ScoreBoardWidget->AddToViewport(20);
			ScoreBoardWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		if (GameMenuWidgetClass)
		{
			GameMenuWidget = CreateWidget<UEPGameMenuWidget>(this, GameMenuWidgetClass);
			GameMenuWidget->AddToViewport();
			GameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (MatchEndWidgetClass)
		{
			MatchEndWidget = CreateWidget<UEPGameEndWidget>(this, MatchEndWidgetClass);
			MatchEndWidget->AddToViewport(30);
			MatchEndWidget->SetVisibility(ESlateVisibility::Collapsed);
		} 

		if (StartMainGameWidgetClass)
		{
			StartMainGameWidget = CreateWidget<UEPBasicTextWidget>(this, StartMainGameWidgetClass);
			StartMainGameWidget->AddToViewport();
			StartMainGameWidget->SetVisibility(ESlateVisibility::Visible);
		}

		// UI와 입력을 관리하기에 클라이언트인 경우에만 등록되는 대리자들
		OnStartMainGame.AddUObject(this, &AEPPlayerController::StartMainGame);
		OnSetupEndMatch.AddUObject(this, &AEPPlayerController::SetupEndMatch);
	}

	// 매치가 시작하기 전 까지는 입력 비활성화
	DisableInput(this);
	SetInputMode(FInputModeGameOnly());

	StartMainGameWidgetHideDelay = 1.0f;
}

void AEPPlayerController::StartMainGame()
{
	// 스코어보드 위젯 업데이트
	bIsStartMainGame = true;
	UpdateScoreBoard();

	EnableInput(this);

	StartMainGameWidget->SetText(FText::FromString(TEXT("START!!!")));
	StartMainGameWidget->SetTextColorAndOpacity(FLinearColor::Red);
	
	// 준비->시작으로 UI를 변경한 이후 1초 뒤에 숨기기
	FTimerHandle StartMainGameWidgetTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StartMainGameWidgetTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			StartMainGameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	), StartMainGameWidgetHideDelay, false);
}

void AEPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(InputComponent)) 
	{
		Input->BindAction(ShowingScoreBoard, ETriggerEvent::Started, this, &AEPPlayerController::ShowScoreBoard);
		Input->BindAction(ShowingScoreBoard, ETriggerEvent::Completed, this, &AEPPlayerController::HideScoreBoard);
		Input->BindAction(ShowingGameMenu, ETriggerEvent::Started, this, &AEPPlayerController::ShowGameMenu);
	}
}

void AEPPlayerController::UpdateScoreBoard()
{
	if (IsLocalController() && ScoreBoardWidget && bIsStartMainGame)
	{
		ScoreBoardWidget->UpdateScoreBoard();
	}
}

void AEPPlayerController::ClientRPCSetupEndMatch_Implementation()
{
	OnSetupEndMatch.Broadcast();
}

void AEPPlayerController::ClientRPCStartMainGame_Implementation()
{
	OnStartMainGame.Broadcast();
}

void AEPPlayerController::SetupEndMatch()
{
	if (IsLocalController())
	{
		// 매치 종료 상태로 위젯 업데이트
		MatchEndWidget->SetVisibility(ESlateVisibility::Visible);
		ScoreBoardWidget->TurnOnGrayBackBoard();
		ShowScoreBoard();

		// 입력 비활성화
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		SetIgnoreMoveInput(true);
		GetPawn()->DisableInput(this);
	}
}

void AEPPlayerController::ClientRPCEndMatch_Implementation()
{
	if (IsLocalController())
	{
		MatchEndWidget->OnReturnToLobbyButtonClicked();
	}
}

void AEPPlayerController::ShowScoreBoard()
{
	if (ScoreBoardWidget)
	{
		ScoreBoardWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AEPPlayerController::HideScoreBoard()
{
	if (ScoreBoardWidget)
	{
		ScoreBoardWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AEPPlayerController::ShowGameMenu()
{	
	if (GameMenuWidget)
	{
		GameMenuWidget->SetVisibility(ESlateVisibility::Visible);
		GetPawn()->DisableInput(this);
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void AEPPlayerController::HideGameMenu()
{
	if (GameMenuWidget)
	{
		GameMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		GetPawn()->EnableInput(this);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AEPPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (IsLocalController())
	{
		// 클라이언트는 서버에게 서버 시간을 요청한다. 이 때 클라이언트의 요청 시간 또한 함께 전달한다.
		ServerRequestServerTime(this, GetWorld()->GetTimeSeconds());
	}
}

void AEPPlayerController::ServerRequestServerTime_Implementation(APlayerController* Requestor, float RequestWorldTime)
{
	// 서버는 서버 시간을 구해 클라이언트에게 전달한다. 클라이언트의 요청 시간 또한 함께 전달한다.
	float WorldServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	ClientReportServerTime(RequestWorldTime, WorldServerTime);
}

void AEPPlayerController::ClientReportServerTime_Implementation(float RequestWorldTime, float RequestServerTime)
{
	/* 
	* 클라이언트는 본인이 요청한 시간과 서버 시간을 전달받는다.
	* 현재 시간에서 클라이언트의 요청 시간을 뺀 값이 클라이언트-서버-클라이언트로 한 번 왕복하는데 소요된 시간이다.
	*/
	float RoundTripTime = GetWorld()->GetTimeSeconds() - RequestWorldTime;

	/*
	* 왕복 시간의 절반은 단방향 전송에 사용되는 시간으로 간주할 수 있다(완벽하게 같지 않을 수 있다).
	* 전달받은 서버 시간은 서버에서 클라이언트로 전송될 때의 시간이 고려되지 않은 값이다. 
	* 따라서 서버 시간에 단방향 전송 시간을 더해야만 클라이언트와 서버에서 동일한 시간을 얻을 수 있다.
	*/
	float AdjustedTime = RequestServerTime + (RoundTripTime * 0.5f);

	/* 클라이언트는 최종적으로 계산한 시간을 '서버시간'으로 사용한다 */
	ServerTime = AdjustedTime;
}


bool AEPPlayerController::ServerRequestServerTime_Validate(APlayerController* Requestor, float RequestWorldTime)
{
	return true;
}




