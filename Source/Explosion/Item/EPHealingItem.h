// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Explosion/Item/EPItemBase.h"
#include "EPHealingItem.generated.h"

/**
 * 체력 회복 아이템
 */

UCLASS()
class EXPLOSION_API AEPHealingItem : public AEPItemBase
{
	GENERATED_BODY()

public:
	AEPHealingItem();
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemValue")
	float HealingAmount;
	
};
