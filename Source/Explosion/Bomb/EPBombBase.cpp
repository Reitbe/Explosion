// Fill out your copyright notice in the Description page of Project Settings.


#include "EPBombBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEPBombBase::AEPBombBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 메시 설정
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMesh;

	// 충돌 설정
	BombMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BombMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AEPBombBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEPBombBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEPBombBase::OnThrowingBomb()
{
}


