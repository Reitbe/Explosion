// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Explosion/UI/EPOverHeadWidget.h"
#include "Explosion/UI/EPHUDWidget.h"
#include "Explosion/Bomb/EPBombManager.h"
#include "Explosion/Stat/EPCharacterStatComponent.h"
#include "Explosion/GameData/EPDeathMatchGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Explosion/Explosion.h"
// EP_LOG(LogExplosion, Log, TEXT("%s"), TEXT("전체 다 몽타주를 재생"));


AEPCharacterBase::AEPCharacterBase()
{
	// 캐릭터와 컨트롤러의 회전을 분리
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 0.2f;

	// Mesh 설정
	GetMesh() ->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Bomb Manager 설정
	BombManager = CreateDefaultSubobject<UEPBombManager>(TEXT("BombManager"));

	// Stat 설정
	StatComponent = CreateDefaultSubobject<UEPCharacterStatComponent>(TEXT("StatComponent"));

	// 캐릭터 상단 UI 설정
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidgetComponent"));
	OverHeadWidgetComponent->SetupAttachment(GetMesh());
	OverHeadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> NameTagWidgetFinder
	(TEXT("/Game/UI/WBP_OverHeadWidget.WBP_OverHeadWidget_C"));
	if (NameTagWidgetFinder.Class)
	{
		OverHeadWidgetComponent->SetWidgetClass(NameTagWidgetFinder.Class);
		OverHeadWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		OverHeadWidgetComponent->SetDrawSize(FVector2D(400.0f, 50.0f));
		OverHeadWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetFinder
	(TEXT("/Game/UI/WBP_HUDWidget.WBP_HUDWidget_C"));
	if (HUDWidgetFinder.Class)
	{
		HUDWidgetClass = HUDWidgetFinder.Class;
	}
}

void AEPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// 게임 인스턴스로부터 정보 가져오기.
	//StatComponent->SetBaseStat();
}

// Called when the game starts or when spawned
void AEPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	// BombManager 초기화	
	if (HasAuthority())
	{
		BombManager->MakeBombObjectPool(BP_Bomb);
		AEPDeathMatchGameMode* GameMode = Cast<AEPDeathMatchGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			StatComponent->OnHpZeroThreeParams.AddUObject(GameMode, &AEPDeathMatchGameMode::OnPlayerKilled);
		}
	}

	// OverHeadWidget 초기화
	OverHeadWidget = Cast<UEPOverHeadWidget>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (OverHeadWidget)
	{
		OverHeadWidget->UpdateNameTag(this); // 임시로 이름 설정
		if (IsLocallyControlled()) 
		{
			OverHeadWidget->SetHpBarVisible(false);
		}
		else
		{
			OverHeadWidget->UpdateHpBar(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
			StatComponent->OnHpChanged.AddUObject(OverHeadWidget, &UEPOverHeadWidget::UpdateHpBar);
		}
	}

	if (HUDWidgetClass && IsLocallyControlled())
	{
		HUDWidget = CreateWidget<UEPHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->UpdateHpBar(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
			StatComponent->OnHpChanged.AddUObject(HUDWidget, &UEPHUDWidget::UpdateHpBar);
		}
	}

	// Delegate 설정
	//StatComponent->OnHpZero.AddUObject(this, &AEPCharacterBase::SetDead);
	StatComponent->OnHpChanged.AddUObject(this, &AEPCharacterBase::TempSetDamaged);
}

// Called every frame
void AEPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (HasAuthority())
	//{
	//	if (IsLocallyControlled())
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("[Server]Server CurrentHp : %f"), StatComponent->GetCurrentHp()));
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("[Server]Client CurrentHp : %f"), StatComponent->GetCurrentHp()));
	//	}
	//}
	//else
	//{
	//	if (IsLocallyControlled())
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("[Client]Server CurrentHp : %f"), StatComponent->GetCurrentHp()));
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("[Client]Client CurrentHp : %f"), StatComponent->GetCurrentHp()));
	//	}
	//}
}

void AEPCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEPCharacterBase::TakeItem(AEPItemBase* NewItemBase)
{
}

void AEPCharacterBase::SetDead()
{
}

void AEPCharacterBase::TempSetDamaged(float CurrentHp, float MaxHp)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("CurrentHp : %f, MaxHp : %f"), CurrentHp, MaxHp));
}

float AEPCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	AController* EventCauser = GetController();
	StatComponent->TakeDamage(Damage, EventCauser, EventInstigator, DamageCauser);

	//AEPDeathMatchGameMode* GameMode = Cast<AEPDeathMatchGameMode>(GetWorld()->GetAuthGameMode());
	//if (GameMode)
	//{
	//	AController* KilledPlayer = GetController();
	//	GameMode->OnPlayerKilled(EventInstigator, KilledPlayer, );
	//}
	return 0.0f;
}

void AEPCharacterBase::OnThrowingBomb()
{
}

void AEPCharacterBase::OnReloadingBomb()
{
}


