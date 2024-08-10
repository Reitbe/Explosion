// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyStatueManager.h"
#include "Explosion/Prop/EPLobbyStatue.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"

// Sets default values for this component's properties
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("플컨-매니저 석상 갯수 : %d"), LobbyStatueWithPedestalsArray.Num()));
}

void UEPLobbyStatueManager::UpdateLobbyStatue(bool IsOnMainMenu)
{
	// 메인메뉴인 경우 석상 전체 비활성화
	if (IsOnMainMenu)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("메인메뉴 - 석상 전체 비활성화!!")));
		for (int32 i = 0; i < LobbyStatueWithPedestalsArray.Num(); ++i)
		{
			AEPLobbyStatue* LobbyStatue = Cast<AEPLobbyStatue>(LobbyStatueWithPedestalsArray[i]);
			if (LobbyStatue)
			{
				LobbyStatue->SetVisibility(false);
				//LobbyStatue->SetActorHiddenInGame(true);
			}
		}
	}
	// 로비인 경우 석상 활성화 여부 갱신
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("로비 석상 비활성화 시작")));

		TArray<TObjectPtr<APlayerState>> PlayerArray;

		// 게임 스테이트 가져오기
		if (GetWorld()->GetGameState<AGameState>() == nullptr)
		{
			// 없다면 1초 후에 다시 시도
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("게임스테이트가 없다.")));
			GetWorld()->GetTimerManager().SetTimer(GameStateIsReadyTimerHandle, FTimerDelegate::CreateLambda(
				[this]() { UpdateLobbyStatue(false); }
			)
			, 0.1f, false, -1.0f);
			return;
		}
		else
		{
			PlayerArray = GetWorld()->GetGameState<AGameState>()->PlayerArray;
		}

		// 플레이어가 석상보다 많은 경우
		if (PlayerArray.Num() > LobbyStatueWithPedestalsArray.Num())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("플레이어가 석상보다 많다. 플레이어:%d, 석상:%d"), PlayerArray.Num(), LobbyStatueWithPedestalsArray.Num()));
			return;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("플레이어:%d, 석상:%d"), PlayerArray.Num(), LobbyStatueWithPedestalsArray.Num()));
		}

		int32 idx = 0;
		// 로비 석상 활성화
		for (; idx < PlayerArray.Num(); ++idx)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("플레이어 석상 활성화!! idx : %d "), idx));
			APlayerState* PlayerState = Cast<APlayerState>(PlayerArray[idx]);
			FString PlayerName = "";
			if (PlayerState)
			{
				PlayerName = PlayerState->GetPlayerName();
			}

			AEPLobbyStatue* LobbyStatueWithPedestal = Cast<AEPLobbyStatue>(LobbyStatueWithPedestalsArray[idx]);
			if (LobbyStatueWithPedestal)
			{
				LobbyStatueWithPedestal->SetVisibility(true);
				LobbyStatueWithPedestal->SetNameTag(PlayerName);
			}
		}

		// 잔여 로비 석상 비활성화 
		for (; idx < LobbyStatueWithPedestalsArray.Num(); ++idx)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("잔여 석상 비활성화중 idx : %d "), idx));
			AEPLobbyStatue* LobbyStatueWithPedestal = Cast<AEPLobbyStatue>(LobbyStatueWithPedestalsArray[idx]);
			if (LobbyStatueWithPedestal)
			{
				LobbyStatueWithPedestal->SetVisibility(false);
			}
		}
	}
}

