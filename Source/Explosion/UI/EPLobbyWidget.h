// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPLobbyWidget.generated.h"

class UButton;
class UTextBlock;
class UEPMultiplayerSessionSubsystem;

/**
 * 플레이어가 세션을 생성하거나, 다른 플레이어의 세션에 참가했을 때 보여지는 로비 위젯.
 * 게임을 시작하기 위한 준비 버튼과 로비로 돌아가기 위한 버튼이 있다.
 */

UCLASS()
class EXPLOSION_API UEPLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/* 총 인원 대비 준비 버튼을 누른 수를 표시하는 위젯 업데이트 */
	void SetPlayerCount(int32 PlayerInSession, int32 MaxPlayer);

protected:
	UFUNCTION()
	void OnReadyButtonClicked();

	UFUNCTION()
	void OnReturnToMainButtonClicked();

	UFUNCTION()
	void OnDestroySessionComplete(bool WasSuccessful);


protected:
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReadyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnToMainButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerCounter;

private:
	/* 세션 생성에 성공했을 때 이동할 레벨의 경로 */
	FString LevelPath;
};
