// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EPScoreBoardWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Explosion/UI/EPBasicTextWidget.h"
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/GameData/EPPlayerState.h"


UEPScoreBoardWidget::UEPScoreBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// 플레이어의 정보를 출력할 내부 위젯 로드
	static ConstructorHelpers::FClassFinder<UEPBasicTextWidget> TextBlockWidgetClassFinder
	(TEXT("/Game/UI/WBP_PlayerScoreWidget.WBP_PlayerScoreWidget_C"));
	if (TextBlockWidgetClassFinder.Class)
	{
		PlayerScoreWidgetClass = TextBlockWidgetClassFinder.Class;
	}
}

void UEPScoreBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEPScoreBoardWidget::UpdateScoreBoard()
{
	// GameState의 ScoreBoard에는 각 플레이어의 PlayerState와 Score가 있으며, Score가 높은 순서대로 정렬되어있다.
	TArray<FScoreBoard> &SB = GetWorld()->GetGameState<AEPGameState>()->ScoreBoard;

	// 정보를 담을 위젯들을 플레이어 수에 맞게 조정한다.
	ResizeScoreBoard(SB.Num());

	for (int32 i = 0; i < SB.Num(); ++i)
	{
		FString PlayerName = SB[i].PlayerState->GetPlayerName();
		int32 KillCount = SB[i].PlayerState->GetKillCount();
		int32 DeathCount = SB[i].PlayerState->GetDeathCount();
		int32 Score = SB[i].Score;

		FText PlayerScoreText = FText::Format(
			NSLOCTEXT("ScoreBoard", "PlayerScoreText", "{0} / Kills: {1} / Deaths: {2} / Score: {3}"), 
			FText::FromString(PlayerName), 
			FText::AsNumber(KillCount), 
			FText::AsNumber(DeathCount), 
			FText::AsNumber(Score)
		);
		PlayerScoreWidgets[i]->SetText(PlayerScoreText);
	}
}

void UEPScoreBoardWidget::ResizeScoreBoard(int32 PlayerCountInGame)
{
	while (PlayerCountInGame != PlayerScoreWidgets.Num())
	{
		// 게임에 참가한 플레이어가 현재 점수 위젯 블럭의 수보다 많은 경우, 위젯을 추가한다.
		if (PlayerCountInGame > PlayerScoreWidgets.Num())
		{
			UEPBasicTextWidget* PlayerScoreWidget = CreateWidget<UEPBasicTextWidget>(GetWorld(), PlayerScoreWidgetClass);
			PlayerScoreWidgets.Add(PlayerScoreWidget);
			PlayerScoreVerticalBox->AddChild(PlayerScoreWidget);
		}
		// 게임에 참가한 플레이어가 현재 점수 위젯 블럭의 수보다 적은 경우, 위젯을 제거한다.
		else if (PlayerCountInGame < PlayerScoreWidgets.Num())
		{
			int32 LastIndex = PlayerScoreWidgets.Num() - 1;
			PlayerScoreVerticalBox->RemoveChild(PlayerScoreWidgets[LastIndex]);
			PlayerScoreWidgets.RemoveAt(LastIndex);
		}
	}
}

void UEPScoreBoardWidget::TurnOnGrayBackBoard()
{
	BackgroundBoarder->SetBrushColor(FLinearColor(0.125f, 0.125f, 0.125f, 0.7)); 
}
