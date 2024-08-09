// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameInstance.h"
#include "GameFramework/GameUserSettings.h"

UEPGameInstance::UEPGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DefaultStatFinder
	(TEXT("/Game/Data/DT_CharacterBaseStat.DT_CharacterBaseStat"));
	if (DefaultStatFinder.Succeeded())
	{
		CharacterStatTable = DefaultStatFinder.Object;
	}
}

void UEPGameInstance::Init()
{
	Super::Init();
	if(CharacterStatTable)
	{
		DefaultStat = *CharacterStatTable->FindRow<FEPCharacterStat>(FName(TEXT("DefaultHp")), TEXT("Find Error"));
	}

	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (GameUserSettings)
	{
		GameUserSettings->SetOverallScalabilityLevel(1);
		GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
		GameUserSettings->ApplySettings(false);
	}
	
}

