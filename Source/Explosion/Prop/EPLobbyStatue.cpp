// Fill out your copyright notice in the Description page of Project Settings.


#include "EPLobbyStatue.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "Explosion/UI/EPNameTagWidget.h"

// Sets default values
AEPLobbyStatue::AEPLobbyStatue()
{
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = CapsuleComponent;

	// 추가될 캐릭터
	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorComponent"));
	ChildActorComponent->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FClassFinder<ACharacter> StatueCharacterFinder
	(TEXT("/Game/Character/BP_LobbyStatue.BP_LobbyStatue_C"));
	if (StatueCharacterFinder.Class)
	{
		StatueCharacter = StatueCharacterFinder.Class;
		ChildActorComponent->SetChildActorClass(StatueCharacter);
	}

	// 캐릭터가 서있을 기단
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StatuePedestal"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StatuePedestalFinder
	(TEXT("/Game/PolygonAncientEmpire/Meshes/Props/SM_Prop_Plinth_05.SM_Prop_Plinth_05"));
	if (StatuePedestalFinder.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(StatuePedestalFinder.Object);
	}

	// 플레이어 이름 표시용 UI
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FClassFinder<UEPNameTagWidget> NameTagWidgetClassFinder
	(TEXT("/Game/UI/WBP_NameTagWidget.WBP_NameTagWidget_C"));
	if (NameTagWidgetClassFinder.Class)
	{
		NameTagWidgetClass = NameTagWidgetClassFinder.Class;
		WidgetComponent->SetWidgetClass(NameTagWidgetClass);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AEPLobbyStatue::SetVisibility(bool bVisible)
{
	if (bVisible)
	{
		ChildActorComponent->SetVisibility(true);
		StaticMeshComponent->SetVisibility(true);
		WidgetComponent->SetVisibility(true);
	}
	else
	{
		ChildActorComponent->SetVisibility(false);
		StaticMeshComponent->SetVisibility(false);
		WidgetComponent->SetVisibility(false);
	}
}

void AEPLobbyStatue::SetNameTag(FString Name)
{
	if (NameTagWidget)
	{
		NameTagWidget->SetNameTag(Name);
	}
}

// Called when the game starts or when spawned
void AEPLobbyStatue::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);

	// OverHeadWidget 초기화
	NameTagWidget = Cast<UEPNameTagWidget>(WidgetComponent->GetUserWidgetObject());
	if (NameTagWidget)
	{
		NameTagWidget->SetNameTag(FString(TEXT("None")));
	}

	SetVisibility(false);
	//SetActorHiddenInGame(true); // 매니저가 인식을 할 수 없다.
}

