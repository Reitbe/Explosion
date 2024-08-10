// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPTimerWidget.generated.h"


UCLASS()
class EXPLOSION_API UEPTimerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetInitialTime(int32 Time) { InitialTime = Time; }
	void StartTimeDisplay();
	void StopTimeDisplay();

protected:
	void UpdateTimeDisplay();
	

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TimeDisplay;

protected:
	FTimerHandle DisplayTimerHandle;
	FTimerDelegate DisplayTimerDelegate;

	TObjectPtr<class AEPGameState> EPGameState;
	TObjectPtr<class AEPPlayerController> EPPlayerController;

	int32 Minutes;
	int32 Seconds;
	float InitialTime;
};
