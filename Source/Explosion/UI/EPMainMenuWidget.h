// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EPMainMenuWidget.generated.h"

class UButton;
class UEPFindSessionWidget;
class UEPCreateSessionWidget;

/**
 * 게임을 실행했을 때 처음 마주하게 되는 메인 메뉴 레벨의 위젯.
 * 로비에서 다시 되돌아오거나, 게임이 종료되었을 때 나타난다.
 * 세션의 생성, 탐색, 혹은 게임 종료를 할 수 있는 버튼들이 있다.
 */

UCLASS()
class EXPLOSION_API UEPMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnPlayGameButtonClicked();

	UFUNCTION()
	void OnExitGameButtonClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PlayGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitGameButton;

	/* 언리얼 온라인 서브시스템을 사용하여 세션 탐색을 진행하기 위한 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEPFindSessionWidget> FindSessionWidget;

	/* 언리얼 온라인 서브시스템을 사용하여 세션 생성을 진행하기 위한 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEPCreateSessionWidget> CreateSessionWidget;

};
