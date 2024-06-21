// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPHpBarWidget.generated.h"

UCLASS()
class EXPLOSION_API UEPHpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);

protected:
	void SetHpBar();
	void SetHpText();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HpText;

	float CurrentHp;
	float MaxHp;
};
