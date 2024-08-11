// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPGameMenuWidget.generated.h"

class UButton;

/**
 * 메인 게임 플레이 도중 띄울 수 있는 메뉴 위젯.
 * 다시 게임으로 돌아가 플레이를 진행하거나, 메인 메뉴로 돌아갈 수 있다.
 */

UCLASS()
class EXPLOSION_API UEPGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReturnToGameButtonClicked();

	UFUNCTION()
	void OnReturnToLobbyButtonClicked();

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnToGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnToLobbyButton;

private:
	/* 메인 메뉴 레벨로 이동하기 위한 레벨 경로 */
	FString LevelPath;
};
