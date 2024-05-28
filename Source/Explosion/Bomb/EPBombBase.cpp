// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBombBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEPBombBase::AEPBombBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);

	// 충돌 설정
	//BombCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BombCollision"));
	//RootComponent = BombCollisionComponent;


	// 메시 설정
	BombMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMeshComponent;

	BombMeshComponent->SetSimulatePhysics(true);
	BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// 충돌 설정
	//BombMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	//BombMeshComponent->SetSimulatePhysics(false);

	BombMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BombMovementComponent"));

	BombParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BombParticleComponent"));
	BombParticleComponent->SetupAttachment(RootComponent);

	BombAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BombAudioComponent"));
	BombAudioComponent->SetupAttachment(RootComponent);


	// 무게 설정(임시)
	BombMass = 20.0f;

}

void AEPBombBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEPBombBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEPBombBase::OnThrowingBomb()
{
	//if (BombMeshComponent) 
	//{
		//BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//BombMeshComponent->SetSimulatePhysics(true);
	//}

}


