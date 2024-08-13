// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPHpBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 플레이어의 체력 상황을 나타내는 위젯
 */

UCLASS()
class EXPLOSION_API UEPHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	/* 체력에 변동 사항이 있을 때 위젯 업데이트 */
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);

protected:
	/* 전체 체력 대비 현재 체력을 퍼센트로 나타내는 프로그래스 바 업데이트*/
	void SetHpBar();

	/* 전체 체력 대비 현재 체력을 텍스트로 나타내는 텍스트 위젯 업데이트*/
	void SetHpText();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText;

private:
	float CurrentHp;
	float MaxHp;
};
