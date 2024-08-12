// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EPCharacterItemInterface.generated.h"

class AEPItemBase;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEPCharacterItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 캐릭터가 아이템과 상호작용할 때 반드시 구현해야 하는 인터페이스
 */
class EXPLOSION_API IEPCharacterItemInterface
{
	GENERATED_BODY()


public:
	/* 아이템의 종류와 상관 없이 상호작용 구현용 */
	virtual void TakeItem(AEPItemBase* NewItemBase) = 0;
};
