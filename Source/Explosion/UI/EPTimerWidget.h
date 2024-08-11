// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPTimerWidget.generated.h"

class UTextBlock;
class AEPGameState;
class AEPPlayerController;

/**
 * 본 게임에서 매치의 잔여 시간을 표시하는 위젯.
 * 클라이언트와 서버의 지연시간을 고려하여 시간 동기화를 유지한다.
 */

UCLASS()
class EXPLOSION_API UEPTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetInitialTime(int32 Time) { InitialTime = Time; }

	/* 위젯에 잔여 시간을 출력하기 시작 */
	void StartTimeDisplay();

	/* 위젯에 시간 출력을 중지 */
	void StopTimeDisplay();

protected:
	/* 1초마다 잔여 시간을 계산하여 위젯 업데이트 */
	void UpdateTimeDisplay();
	

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeDisplay;

protected:
	/* 1초 간격 호출을 위한 SetTimer용 대리자 핸들 */
	FTimerHandle DisplayTimerHandle;
	FTimerDelegate DisplayTimerDelegate;

	/* 지정된 매치 진행 시간 가져오기용 */
	TObjectPtr<AEPGameState> EPGameState;

	/* 서버-클라이언트간 시간 차이 계산용 */
	TObjectPtr<AEPPlayerController> EPPlayerController;

	int32 Minutes;
	int32 Seconds;

	/* 위젯에 표시할 총 시간*/
	float InitialTime;
};
