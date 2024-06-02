// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBombBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AEPBombBase::AEPBombBase()
{
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
	SetReplicateMovement(true);

	// 메시 설정
	BombMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMeshComponent;

	// 충돌 설정-폭탄과 캐릭터 초기 충돌 방지
	BombMeshComponent->SetSimulatePhysics(false);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BombMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BombMovementComponent"));

	BombAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BombAudioComponent"));
	BombAudioComponent->SetupAttachment(RootComponent);

	// 옵션 설정(임시)
	BombMass = 20.0f;
	BombDelayTime = 3.0f;
}

void AEPBombBase::BeginPlay()
{
	Super::BeginPlay();
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
}

void AEPBombBase::DeactiveBomb()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	BombMeshComponent->SetSimulatePhysics(false);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AEPBombBase::ActiveBombTimeTrigger()
{
	if (HasAuthority())
	{
		FTimerHandle DestroyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AEPBombBase::MulticastRPCExplode, BombDelayTime, false);
	}
}

void AEPBombBase::MulticastRPCExplode_Implementation()
{
	BombGamePlayStatics->SpawnEmitterAtLocation(GetWorld(), BombParticleSystem, GetActorLocation());
	DeactiveBomb();
}



