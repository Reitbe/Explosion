// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPCreateSessionWidget.generated.h"

class UButton;
class UCheckBox;
class UEPMultiplayerSessionSubsystem;

/**
 * 로비에서 세션 생성을 진행하기 위한 위젯.
 * 생성 조건으로 매치의 최대 인원과 LAN 사용 여부를 설정할 수 있다.
 * 세션이 생성되었다면 로비 레벨로 이동한다.
 */

UCLASS()
class EXPLOSION_API UEPCreateSessionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnLANCheckBoxStateChanged(bool bIsChecked);
	
	/* 매치 최대 인원으로 2인 버튼 클릭 시 실행 */
	UFUNCTION()
	void OnSelectTwoPlayerButtonClicked();

	/* 매치 최대 인원으로 4인 버튼 클릭 시 실행 */
	UFUNCTION()
	void OnSelectFourPlayerButtonClicked();

	UFUNCTION()
	void OnCreateSessionButtonClicked();

	UFUNCTION()
	void OnCreateSessionComplete(bool bWasSuccessful);

protected:
	UEPMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectTwoPlayerButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectFourPlayerButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> LANCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateSessionButton;
	
private:
	/* 세션 생성 조건으로 사용되는 매치에 참여 가능한 최대 인원 수 */
	int32 MaxPlayerNum;

	/* 세션 생성 조건으로 사용되는 LAN 사용 여부 */
	uint8 bUseLAN : 1;

	/* 세션 생성 조건으로 사용되는 매치 타입 */
	FString TypeOfMatch = FString(TEXT("FreeForAll"));

	/* 세션 생성 완료 시 이동할 레벨 경로*/
	FString LevelPath;
};
