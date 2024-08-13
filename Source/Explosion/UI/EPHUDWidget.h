// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPHUDWidget.generated.h"

class UVerticalBox;
class UEPTimerWidget;
class UEPHpBarWidget;

/**
 * EPCharacterBase에 부착되는 HUD 위젯
 * 플레이어의 체력, 잔여 매치 시간, 차징이 가능한 경우 차징 상태를 표시한다.
 */

UCLASS()
class EXPLOSION_API UEPHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/* 체력 상태에 변동이 있는 경우 위젯을 업데이트 */
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);

	void StartTimer();

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ChargingBarFrame;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEPTimerWidget> TimerWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEPHpBarWidget> HpBarWidget;
};
