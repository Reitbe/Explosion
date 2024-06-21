// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPNameTagWidget.generated.h"

UCLASS()
class EXPLOSION_API UEPNameTagWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	void SetNameTag(FString Name);
	//임시로 네트워크 상에서 플레이어의 역할을 나타낸다
	void ShowPlayerNetRole(TObjectPtr<APawn> Player);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtNameTag;
};
