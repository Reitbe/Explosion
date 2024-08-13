// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBombBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AEPBombBase::AEPBombBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	// 메시 설정
	BombMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMeshComponent;

	// 충돌 설정-폭탄과 캐릭터 초기 충돌 방지
	BombMeshComponent->SetIsReplicated(true);
	BombMeshComponent->SetSimulatePhysics(false);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BombAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BombAudioComponent"));
	BombAudioComponent->SetupAttachment(RootComponent);

}

void AEPBombBase::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
}

void AEPBombBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEPBombBase::ActiveBomb()
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	BombMeshComponent->SetSimulatePhysics(true);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	bIsBombActive = true;
}

void AEPBombBase::DeactiveBomb()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	BombMeshComponent->SetSimulatePhysics(false);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsBombActive = false;
}

void AEPBombBase::SetBombInHandOption()
{
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	BombMeshComponent->SetSimulatePhysics(false);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEPBombBase::ActiveBombTimeTrigger()
{
	if (HasAuthority())
	{
		FTimerHandle BombExplosionTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(BombExplosionTimerHandle, this, &AEPBombBase::MulticastRPCExplode, BombDelayTime, false);
	}
}

void AEPBombBase::MulticastRPCExplode_Implementation()
{
	// 모든 클라이언트에서 폭발 이펙트 생성
	BombGamePlayStatics->SpawnEmitterAtLocation(GetWorld(), BombParticleSystem, GetActorLocation());

	// 서버는 폭발 데미지까지 처리
	if (HasAuthority())
	{
		DeactiveBomb();
		AController* BombOwnerController = GetOwner()->GetInstigatorController();
		BombGamePlayStatics->ApplyRadialDamage(GetWorld(), BombDamage, GetActorLocation(), BombAreaRadius, nullptr, TArray<AActor*>(), this, BombOwnerController, true, ECC_Visibility);
	}
}



