// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "EPScoreBoardWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Explosion/UI/EPBasicTextWidget.h"
#include "Explosion/GameData/EPGameState.h"
#include "Explosion/GameData/EPPlayerState.h"


UEPScoreBoardWidget::UEPScoreBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> TextBlockWidgetClassFinder
	(TEXT("/Game/UI/WBP_PlayerScoreWidget.WBP_PlayerScoreWidget_C"));
	if (TextBlockWidgetClassFinder.Class)
	{
		PlayerScoreWidgetClass = TextBlockWidgetClassFinder.Class;
	}
}

void UEPScoreBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateScoreBoard();
}

void UEPScoreBoardWidget::UpdateScoreBoard()
{
	TArray<FScoreBoard> &SB = GetWorld()->GetGameState<AEPGameState>()->ScoreBoard;
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

void UEPScoreBoardWidget::ResizeScoreBoard(int32 scoreBoardSize)
{
	while (scoreBoardSize != PlayerScoreWidgets.Num())
	{
		if (scoreBoardSize > PlayerScoreWidgets.Num())
		{
			UEPBasicTextWidget* PlayerScoreWidget = CreateWidget<UEPBasicTextWidget>(GetWorld(), PlayerScoreWidgetClass);
			PlayerScoreWidgets.Add(PlayerScoreWidget);
			PlayerScoreVerticalBox->AddChild(PlayerScoreWidget);
		}
		else if (scoreBoardSize < PlayerScoreWidgets.Num())
		{
			int32 LastIndex = PlayerScoreWidgets.Num() - 1;
			PlayerScoreVerticalBox->RemoveChild(PlayerScoreWidgets[LastIndex]);
			PlayerScoreWidgets.RemoveAt(LastIndex);
		}
	}
}
