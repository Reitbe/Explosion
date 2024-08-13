// Fill out your copyright notice in the Description page of Project Settings.


#include "EPItemBase.h"
#include "Engine/StaticMesh.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Explosion/Interface/EPCharacterItemInterface.h"
#include "Particles/ParticleSystemComponent.h"

AEPItemBase::AEPItemBase()
{
 	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemBaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemBaseMeshComponent"));
	
	CapsuleTriggerComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleTriggerComponent"));
	
	OverlapEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemBaseMeshFinder
	(TEXT("/Game/PolygonAncientEmpire/Meshes/Props/SM_Prop_Pillow_04.SM_Prop_Pillow_04"));
	if (ItemBaseMeshFinder.Succeeded())
	{
		ItemBaseMeshComponent->SetStaticMesh(ItemBaseMeshFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectFinder
	(TEXT("/Game/InfinityBladeEffects/Effects/FX_Ability/Heal/P_Health_Pickup_01.P_Health_Pickup_01"));
	if (EffectFinder.Succeeded())
	{
		OverlapEffectComponent->SetTemplate(EffectFinder.Object);
		OverlapEffectComponent->bAutoActivate = false;
	}

	RootComponent = CapsuleTriggerComponent;
	ItemMeshComponent->SetupAttachment(RootComponent);
	ItemBaseMeshComponent->SetupAttachment(RootComponent);
	OverlapEffectComponent->SetupAttachment(RootComponent);

	CapsuleTriggerComponent->SetCapsuleSize(50.f, 50.f);
	CapsuleTriggerComponent->SetCollisionProfileName(TEXT("Item"));
	ItemBaseMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
	ItemBaseMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	ItemMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	OverlapEffectComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));

}

void AEPItemBase::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
}

void AEPItemBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CapsuleTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &AEPItemBase::OnOverlapBegin);
}


void AEPItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	// 충돌이 발생한 캐릭터의 TakeItem 함수 호출
	IEPCharacterItemInterface* OverlappedPlayer = Cast<IEPCharacterItemInterface>(OtherActor);
	if (OverlappedPlayer)
	{
		OverlappedPlayer->TakeItem(this);
	}

	OverlapEffectComponent->Activate(true);
	OverlapEffectComponent->OnSystemFinished.AddDynamic(this, &AEPItemBase::OnEffectFinished);

	ItemBaseMeshComponent->SetHiddenInGame(true);
	ItemMeshComponent->SetHiddenInGame(true);
	CapsuleTriggerComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void AEPItemBase::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

