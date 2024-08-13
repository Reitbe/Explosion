// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPBasicTextWidget.generated.h"

class UTextBlock;

/**
 * 다용도로 사용 가능한 기본 텍스트 위젯.
 * 텍스트 내용과 색상을 설정할 수 있다.
 */
 
UCLASS()
class EXPLOSION_API UEPBasicTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetText(FText Text);
	void SetTextColorAndOpacity(FLinearColor ColorAndOpacity);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BasicTextBlock;
};
