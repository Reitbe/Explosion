// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Explosion/EPCharacterStat.h"
#include "EPGameInstance.generated.h"

/**
 * 게임시 처음 시작되고 나서 종료될 때 까지 유지되어야 하는 정보들, 혹은 게임 전체에서 공유되어야 하는 정보들을 저장하는 클래스.
 * 게임에 참가한 총 인원과 캐릭터의 기본 스탯을 저장한다.
 * 게임의 그래픽 퀄리티 또한 여기서 설정한다.
 */

UCLASS()
class EXPLOSION_API UEPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEPGameInstance();
	virtual void Init() override;

public:
	int32 GetPlayerCount() { return PlayerCount; };
	void SetPlayerCount(int32 NewPlayerCount) { PlayerCount = NewPlayerCount; };

	/* 데이터테이블에서 가져온 캐릭터의 기본 스텟 정보를 반환 */
	FEPCharacterStat GetDefaultCharacterStat() { return DefaultStat; };

protected:
	/* 캐릭터의 기본 스텟을 저장해둔 데이터 테이블 */
	TObjectPtr<UDataTable> CharacterStatTable;

	/* CharacterStatTable에서 가져온 데이터를 담을 변수*/
	FEPCharacterStat DefaultStat;

private:
	/* 게임에 참가한 플레이어 총 인원. 로비에서 업데이트하고 메인 게임에서 사용한다.*/
	int32 PlayerCount{ 0 };
};
