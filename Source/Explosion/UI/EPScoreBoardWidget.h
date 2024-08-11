// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPScoreBoardWidget.generated.h"

class UVerticalBox;
class UBorder;
class UEPBasicTextWidget;

/**
 * 본 게임 도중 or 게임 종료 후에 각 플레이어의 점수를 보여주는 위젯. 플레이어 이름, 킬, 데스, 점수를 보여준다.
 * 플레이어 사망 이후에 정보가 갱신되면 그 때 본 위젯을 업데이트한다.
 * 상단에 위치한 플레이어가 1등이며, 아래로 갈수록 순위가 내려간다.
 */

UCLASS()
class EXPLOSION_API UEPScoreBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEPScoreBoardWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	
public:
	/* 각 플레이어의 이름, 킬, 데스, 스코어 정보를 갱신하여 위젯에 반영한다 */
	void UpdateScoreBoard();

	/* 게임에 참가한 플레이어의 수에 맞추어 스코어보드 위젯의 갯수를 조정한다*/
	void ResizeScoreBoard(int32 PlayerCountInGame);

	/* 매치가 종료되었을 때 스코어보드를 강조하기 위하여 회색 배경을 활성화한다 */
	void TurnOnGrayBackBoard();
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> PlayerScoreVerticalBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundBoarder;

	/* 플레이어의 정보를 출력할 내부 위젯 블럭*/
	TSubclassOf<UEPBasicTextWidget> PlayerScoreWidgetClass;

	/* UEPBasicTextWidget을 현재 플레이어의 수 만큼 보유한 스코어보드*/
	TArray<TObjectPtr<UEPBasicTextWidget>> PlayerScoreWidgets;
};
