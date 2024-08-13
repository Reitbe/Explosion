// Fill out your copyright notice in the Description page of Project Settings.


#include "EPHealingItem.h"

AEPHealingItem::AEPHealingItem()
{
}

void AEPHealingItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Type = EItemType::IT_Health;
	ItemStat.MaxHp = HealingAmount;
}
