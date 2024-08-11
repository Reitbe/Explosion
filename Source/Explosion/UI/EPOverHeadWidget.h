// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPOverHeadWidget.generated.h"

class UEPHpBarWidget;
class UEPNameTagWidget;

/**
 *  메인 게임의 캐릭터 플레이어 상단에 위치한 위젯이다.
 *  플레이어의 이름과 체력을 표시한다.
 */

UCLASS()
class EXPLOSION_API UEPOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetHpBarVisible(bool bVisible);

	/* 체력 변동이 있는 경우 위젯을 업데이트 */
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);

	/* 플레이어 이름이 준비되면 해당 이름으로 위젯을 업데이트 */
	void UpdateNameTag(FString Name);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEPHpBarWidget> HpBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEPNameTagWidget> NameTagWidget;
};
