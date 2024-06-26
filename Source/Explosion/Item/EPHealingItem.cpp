// Fill out your copyright notice in the Description page of Project Settings.


#include "EPHealingItem.h"

AEPHealingItem::AEPHealingItem()
{
	Type = EItemType::IT_Health;
	ItemStat.MaxHp = HealingAmount;
}

void AEPHealingItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Type = EItemType::IT_Health;
	ItemStat.MaxHp = HealingAmount;
}
