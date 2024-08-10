// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EPCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FEPCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FEPCharacterStat() : MaxHp(100.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	//FEPCharacterStat operator+(const FEPCharacterStat& Other) const
	//{
	//	const float* const ThisPtr = reinterpret_cast<const float* const>(this);
	//	const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

	//	FEPCharacterStat Result;
	//	float* ResultPtr = reinterpret_cast<float*>(&Result);
	//	int32 StatNum = sizeof(FEPCharacterStat) / sizeof(float);
	//	for (int32 i = 0; i < StatNum; i++)
	//	{
	//		ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
	//	}

	//	return Result;
	//}
};