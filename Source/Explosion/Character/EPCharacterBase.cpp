// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Explosion/UI/EPOverHeadWidget.h"
#include "Explosion/UI/EPHUDWidget.h"
#include "Explosion/Bomb/EPBombManager.h"
#include "Explosion/Stat/EPCharacterStatComponent.h"
#include "Explosion/GameData/EPDeathMatchGameMode.h"
#include "Explosion/GameData/EPPlayerState.h"
#include "Explosion/Player/EPPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
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
	//GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

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
		// 플레이어 정보가 준비되는 것을 기다리는 2초
		FTimerHandle CharacterPlayerBeginPlayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(CharacterPlayerBeginPlayTimerHandle, this, &AEPCharacterBase::SetOverHeadPlayerNameUI, 2.0f, false, -1.0f);
		
		// 로컬 플레이어인 경우에는 오버헤드 위젯을 숨김
		if (IsLocallyControlled()) 
		{
			OverHeadWidget->SetVisibility(ESlateVisibility::Collapsed);
			//OverHeadWidget->SetHpBarVisible(false);
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
			HUDWidget->AddToViewport(10);
			HUDWidget->UpdateHpBar(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
			StatComponent->OnHpChanged.AddUObject(HUDWidget, &UEPHUDWidget::UpdateHpBar);
		}
	}

	// 모든 준비가 완료되었음을 서버에 알림
	if (IsLocallyControlled())
	{
		AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetController());
		if (EPPlayerController)
		{
			EPPlayerController->OnStartMainGame.AddUObject(this, &AEPCharacterBase::StartMainGame);
		}
		ServerRPC_PlayerReady();
	}

	DisableInput(GetWorld()->GetFirstPlayerController());
	// Delegate 설정
	//StatComponent->OnHpZero.AddUObject(this, &AEPCharacterBase::SetDead);
	StatComponent->OnHpChanged.AddUObject(this, &AEPCharacterBase::TempSetDamaged);
}

// Called every frame
void AEPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void AEPCharacterBase::StartMainGame()
{
	// 입력 활성화
	EnableInput(GetWorld()->GetFirstPlayerController());

	// 타이머 위젯 시작
	if (HUDWidget)
	{
		HUDWidget->StartTimer();
	}
}

void AEPCharacterBase::SetOverHeadPlayerNameUI()
{
	FString PlayerName = "";
	AEPPlayerState* EPPlayerState = GetPlayerState<AEPPlayerState>();
	if (EPPlayerState)
	{
		PlayerName = EPPlayerState->GetPlayerName();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("플레이어 스테이트가 없습니다.")));
	}
	OverHeadWidget->UpdateNameTag(PlayerName);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("캐플 BeginPlay - 플레이어 이름 : %s"), *PlayerName));

}

void AEPCharacterBase::ServerRPC_PlayerReady_Implementation()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	AEPDeathMatchGameMode* EPGameMode = Cast<AEPDeathMatchGameMode>(GameMode);
	if (EPGameMode)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("캐릭터고 서버에 준비 신호 보낸다.")));
		EPGameMode->CheckAllPlayersReady();
	}
}


