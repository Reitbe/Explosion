// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPHUDWidget.generated.h"


UCLASS()
class EXPLOSION_API UEPHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEPHpBarWidget> HpBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEPTimerWidget> TimerWidget;
};
