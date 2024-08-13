// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyStatueManager.h"
#include "Explosion/Prop/EPLobbyStatue.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"

UEPLobbyStatueManager::UEPLobbyStatueManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AEPLobbyStatue> LobbyStatueClassFinder
	(TEXT("/Game/Map/BP_LobbyStatueWithPedestal.BP_LobbyStatueWithPedestal_C"));
	if (LobbyStatueClassFinder.Class)
	{
		LobbyStatueWithPedestalsClass = LobbyStatueClassFinder.Class;
	}
}

void UEPLobbyStatueManager::GetAllLobbyStatue()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), LobbyStatueWithPedestalsClass, LobbyStatueWithPedestalsArray);
}

void UEPLobbyStatueManager::UpdateLobbyStatue(bool IsOnMainMenu)
{
	// 메인메뉴인 경우 석상 전체 비활성화
	if (IsOnMainMenu)
	{
		for (int32 i = 0; i < LobbyStatueWithPedestalsArray.Num(); ++i)
		{
			AEPLobbyStatue* LobbyStatue = Cast<AEPLobbyStatue>(LobbyStatueWithPedestalsArray[i]);
			if (LobbyStatue)
			{
				LobbyStatue->SetVisibility(false);
			}
		}
	}
	// 로비인 경우 석상 활성화 업데이트
	else
	{
		// 매치에 참가한 플레이어 목록을 저장할 배열
		TArray<TObjectPtr<APlayerState>> PlayerArray;

		// 게임 스테이트가 생성될 때 까지 반복적으로 접근
		if (GetWorld()->GetGameState<AGameState>() == nullptr)
		{
			GetWorld()->GetTimerManager().SetTimer(GameStateIsReadyTimerHandle, FTimerDelegate::CreateLambda(
				[this]() { UpdateLobbyStatue(false); }
			)
			, 0.1f, false, -1.0f);
			return;
		}
		// 게임 스테이트가 존재한다면 플레이어 목록을 가져온다
		else
		{
			PlayerArray = GetWorld()->GetGameState<AGameState>()->PlayerArray;
		}

		// 석상 인식이 되지 않은 경우이다. 석상은 4개 고정이며 플레이어는 4명이하로 제한되어있기 때문에 플레이어가 석상보다 많을 수 없다. 
		if (PlayerArray.Num() > LobbyStatueWithPedestalsArray.Num())
		{
			return;
		}

		// 석상 번호를 표현하기 위한 인덱스
		int32 StatueIndex = 0;
		
		// 플레이어 수 만큼 석상 활성화
		for (; StatueIndex < PlayerArray.Num(); ++StatueIndex)
		{
			APlayerState* PlayerState = Cast<APlayerState>(PlayerArray[StatueIndex]);
			FString PlayerName = "";
			if (PlayerState)
			{
				PlayerName = PlayerState->GetPlayerName();
			}

			AEPLobbyStatue* LobbyStatueWithPedestal = Cast<AEPLobbyStatue>(LobbyStatueWithPedestalsArray[StatueIndex]);
			if (LobbyStatueWithPedestal)
			{
				LobbyStatueWithPedestal->SetVisibility(true);
				LobbyStatueWithPedestal->SetNameTag(PlayerName);
			}
		}

		// 잔여 석상 비활성화 
		for (; StatueIndex < LobbyStatueWithPedestalsArray.Num(); ++StatueIndex)
		{
			AEPLobbyStatue* LobbyStatueWithPedestal = Cast<AEPLobbyStatue>(LobbyStatueWithPedestalsArray[StatueIndex]);
			if (LobbyStatueWithPedestal)
			{
				LobbyStatueWithPedestal->SetVisibility(false);
			}
		}
	}
}

