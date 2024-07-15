// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPBasicTextWidget.generated.h"


UCLASS()
class EXPLOSION_API UEPBasicTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetText(FText Text);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BasicTextBlock;
};
