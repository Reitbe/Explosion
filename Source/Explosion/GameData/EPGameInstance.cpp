// Fill out your copyright notice in the Description page of Project Settings.


#include "EPGameInstance.h"
#include "GameFramework/GameUserSettings.h"

UEPGameInstance::UEPGameInstance()
{
	// 캐릭터의 기본 스탯을 저장해둔 데이터 테이블 로드
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

	// 데이터테이블에서 기본 체력 항복을 가져온다.
	if(CharacterStatTable)
	{
		DefaultStat = *CharacterStatTable->FindRow<FEPCharacterStat>(FName(TEXT("DefaultHp")), TEXT("Find Error"));
	}

	// 저사양 테스트를 위하여 그래픽 품질을 낮게 설정.
	UGameUserSettings* GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (GameUserSettings)
	{
		GameUserSettings->SetOverallScalabilityLevel(1);
		GameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
		GameUserSettings->ApplySettings(true);
	}
	
}

