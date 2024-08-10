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
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr) {
		Subsystem->AddMappingContext(InputMappingContext, 0);
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

		// 클라이언트인 경우에만 등록되는 대리자들
		OnStartMainGame.AddUObject(this, &AEPPlayerController::StartMainGame);
		OnSetupEndMatch.AddUObject(this, &AEPPlayerController::SetupEndMatch);

		ConsoleCommand(TEXT("stat fps"), true);
	}

	//EnableInput(this);
	DisableInput(this);
	SetInputMode(FInputModeGameOnly());
}

void AEPPlayerController::StartMainGame()
{
	// 스코어보드 위젯 업데이트
	bIsStartMainGame = true;
	UpdateScoreBoard();

	// 움직임 종료 해제
	EnableInput(this);

	// 시작 UI 출력 및 숨기기
	StartMainGameWidget->SetText(FText::FromString(TEXT("START!!!")));
	StartMainGameWidget->SetTextColorAndOpacity(FLinearColor::Red);
	
	FTimerHandle StartMainGameWidgetTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StartMainGameWidgetTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			StartMainGameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	), 1.0f, false);
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

//void AEPPlayerController::ClientRPCUpdateScoreBoard_Implementation()
void AEPPlayerController::UpdateScoreBoard()
{
	if (IsLocalController() && ScoreBoardWidget && bIsStartMainGame)
	{
		bool bIsScoreBoardUpdated = false;
		bool bIsKillCountUpdated = false;
		bool bIsDeathCountUpdated = false;

		//AEPGameState* EPGameState = GetWorld()->GetGameState<AEPGameState>();
		//if (EPGameState)
		//{
		//	bIsScoreBoardUpdated = EPGameState->GetIsScoreBoardReplicated();
		//}

		//AEPPlayerState* EPPlayerState = GetPlayerState<AEPPlayerState>();
		//if (EPPlayerState)
		//{
		//	bIsKillCountUpdated = EPPlayerState->GetIsKillCountReplicated();
		//	bIsDeathCountUpdated = EPPlayerState->GetIsDeathCountReplicated();
		//}

		if (true || (bIsScoreBoardUpdated && bIsKillCountUpdated && bIsDeathCountUpdated))
		{
			//EPGameState->SetIsScoreBoardReplicated(false);
			//EPPlayerState->SetIsKillCountReplicated(false);
			//EPPlayerState->SetIsDeathCountReplicated(false);

			ScoreBoardWidget->UpdateScoreBoard();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ClientRPCUpdateScoreBoard_Implementation"));
		}
	}
}

void AEPPlayerController::ClientRPCSetupEndMatch_Implementation()
{
	OnSetupEndMatch.Broadcast();
}

void AEPPlayerController::ClientRPC_StartMainGame_Implementation()
{
	OnStartMainGame.Broadcast();
}

void AEPPlayerController::SetupEndMatch()
{

	if (IsLocalController())
	{
		MatchEndWidget->SetVisibility(ESlateVisibility::Visible);
		ScoreBoardWidget->TurnOnGrayBackBoard();
		ShowScoreBoard();
	}
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
	//DisableInput(this);
	SetIgnoreMoveInput(true);
	GetPawn()->DisableInput(this);
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
		ServerRequestServerTime(this, GetWorld()->GetTimeSeconds());
	}
}

void AEPPlayerController::ClientReportServerTime_Implementation(float requestWorldTime, float serverTime)
{
	float roundTripTime = GetWorld()->GetTimeSeconds() - requestWorldTime;
	float adjustedTime = serverTime + (roundTripTime * 0.5f);
	ServerTime = adjustedTime;
}

void AEPPlayerController::ServerRequestServerTime_Implementation(APlayerController* requestor, float requestWorldTime)
{
	float serverTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	ClientReportServerTime(requestWorldTime, serverTime);
}

bool AEPPlayerController::ServerRequestServerTime_Validate(APlayerController* requestor, float requestWorldTime)
{
	return true;
}




