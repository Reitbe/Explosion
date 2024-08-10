// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPOverHeadWidget.generated.h"


UCLASS()
class EXPLOSION_API UEPOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetHpBarVisible(bool bVisible);
	void UpdateHpBar(float NewCurrentHp, float NewMaxHp);
	void UpdateNameTag(FString Name);
	//void UpdateNameTag(const FString& NewName);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEPHpBarWidget> HpBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEPNameTagWidget> NameTagWidget;
};
