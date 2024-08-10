// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBattleRoyalGameMode.h"
#include "GameFramework/PlayerStart.h"

AEPBattleRoyalGameMode::AEPBattleRoyalGameMode()
{
}

FTransform AEPBattleRoyalGameMode::GetRandomStartTransform()
{
    if (PlayerStartArray.Num() == 0)
    {
        return FTransform(FVector(0.0f, 0.0f, 400.0f));
    }

    int32 RandomIndex = FMath::RandRange(0, PlayerStartArray.Num() - 1);
    return PlayerStartArray[RandomIndex]->GetActorTransform();
}

void AEPBattleRoyalGameMode::OnPlayerKilled(AController* KillerPlayer, AController* KilledPlayer, AActor* DamageCauser)
{
}
