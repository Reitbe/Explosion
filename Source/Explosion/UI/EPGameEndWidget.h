// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPGameEndWidget.generated.h"

class UButton;

/**
 * 매치가 종료되었음을 알리는 위젯
 */

UCLASS()
class EXPLOSION_API UEPGameEndWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReturnToLobbyButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ReturnToLobbyButton;

private:
	/* 매치 종료 후 메인 메뉴로 돌아가기 위한 레벨 경로 */
	FString LevelPath;


};
