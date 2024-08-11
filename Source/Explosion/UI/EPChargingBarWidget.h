// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPChargingBarWidget.generated.h"

class UProgressBar;

/**
 * 플레이어 캐릭터가 조준 버튼을 누르고 있을 때 등장하는 차징 바 위젯.
 * 차징 정도에 따라 차징 바가 차오르며, 색이 변화한다.
 */

UCLASS()
class EXPLOSION_API UEPChargingBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	/* 최대 차징 기간이 주어졌을 때, 해당 기간 대비 차징 시작 시간의 비율로 위젯을 업데이트 */
	void StartChargingBar(float MaxChargingDuration);

	void EndChargingBar();


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> ChargingBar;

	FTimerHandle ChargingTimerHandle;

private:	
	/* 반복적으로 차징 바를 업데이트하기 위한 시간 간격 */
	float RepeatTimeInterval;

	/* 차징을 시작하고 현재까지 흐른 시간*/
	float ElapsedTime;

	/* 차징 바의 색상을 결정하는 알파값 */
	float AlphaValue;

};
