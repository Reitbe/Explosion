// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPChargingBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPChargingBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void StartChargingBar(float ChargingDuration);
	void EndChargingBar();

protected:

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ChargingBar;

	int32 MaxChargingValue;
	int32 CurrentChargingValue;

	FTimerHandle ChargingTimerHandle;
	
	float RepeatTimeInterval;
	float ElapsedTime;
	float AlphaValue;

};
