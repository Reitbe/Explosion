// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPNameTagWidget.generated.h"

/**
 * 플레이어의 이름을 표시하는 단순한 이름표 위젯
 */

UCLASS()
class EXPLOSION_API UEPNameTagWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	void SetNameTag(FString Name);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TxtNameTag;
};
