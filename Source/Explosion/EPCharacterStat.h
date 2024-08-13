// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EPCharacterStat.generated.h"

/**
 * 캐릭터의 스탯을 저장하는 구조체. 
 * 데이터테이블의 기본 Row로 사용된다.
 */

USTRUCT(BlueprintType)
struct FEPCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEPCharacterStat() : MaxHp(100.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;
};