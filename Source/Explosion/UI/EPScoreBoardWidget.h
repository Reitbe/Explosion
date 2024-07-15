// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPScoreBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPScoreBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UEPScoreBoardWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	
public:
	void UpdateScoreBoard();
	void ResizeScoreBoard(int32 scoreBoardSize);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> PlayerScoreVerticalBox;

	TSubclassOf<class UEPBasicTextWidget> PlayerScoreWidgetClass;
	TArray<TObjectPtr<class UEPBasicTextWidget>> PlayerScoreWidgets;
};
