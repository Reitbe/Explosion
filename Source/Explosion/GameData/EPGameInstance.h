// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Explosion/EPCharacterStat.h"
#include "EPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXPLOSION_API UEPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEPGameInstance();
	virtual void Init() override;

	FEPCharacterStat GetDefaultCharacterStat() { return DefaultStat; };

protected:
	TObjectPtr<UDataTable> CharacterStatTable;
	FEPCharacterStat DefaultStat;

};