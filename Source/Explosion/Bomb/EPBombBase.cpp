// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBombBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AEPBombBase::AEPBombBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 메시 설정
	BombMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMeshComponent;

	// 충돌 설정
	BombMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	BombMeshComponent->SetSimulatePhysics(false);
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
	if (BombMeshComponent) 
	{
		BombMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BombMeshComponent->SetSimulatePhysics(true);
	}

}


