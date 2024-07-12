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
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/GameData/EPPlayerState.h"



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

void AEPPlayerController::ShowScoreBoard()
{
	if (IsLocalController() && ScoreBoardWidget)
	{
		ScoreBoardWidget->AddToViewport();
	}
}

void AEPPlayerController::HideScoreBoard()
{
	if (IsLocalController() && ScoreBoardWidget)
	{
		ScoreBoardWidget->RemoveFromParent();
	}
}

void AEPPlayerController::ShowGameMenu()
{	
	AEPGameState* EPGameState = GetWorld()->GetGameState<AEPGameState>();
	if (EPGameState->ScoreBoard.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ScoreBoard is Empty"));
	}
	for (auto& PlayerScore : EPGameState->ScoreBoard)
	{
		AEPPlayerState* EPPlayerState = Cast<AEPPlayerState>(PlayerScore.Key);
		if (EPPlayerState)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player: %s, Score: %d"), *EPPlayerState->GetPlayerName(), PlayerScore.Value));
		}
	}


	if (IsLocalController() && GameMenuWidget)
	{
		if(GameMenuWidget->IsInViewport())
		{
			GameMenuWidget->RemoveFromParent();
		}
		else
		{
			GameMenuWidget->AddToViewport();
		}
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

void AEPPlayerController::MulticastRPCSetupEndMatch_Implementation()
{
	if (IsLocalController())
	{
		MatchEndWidget = CreateWidget<UUserWidget>(this, MatchEndWidgetClass);
		if (MatchEndWidget)
		{
			MatchEndWidget->AddToViewport();
		}
	}
	DisableInput(this);
	GetPawn()->DisableInput(this);
}


