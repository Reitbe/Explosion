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
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	// Bomb Manager 설정
	BombManager = CreateDefaultSubobject<UEPBombManager>(TEXT("BombManager"));

	// Stat 설정
	StatComponent = CreateDefaultSubobject<UEPCharacterStatComponent>(TEXT("StatComponent"));

	// 캐릭터 상단 UI 컴포넌트 설정
	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverHeadWidgetComponent"));
	OverHeadWidgetComponent->SetupAttachment(GetMesh());
	OverHeadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));

	// 캐릭터 상단 UI 설정
	static ConstructorHelpers::FClassFinder<UUserWidget> NameTagWidgetFinder
	(TEXT("/Game/UI/WBP_OverHeadWidget.WBP_OverHeadWidget_C"));
	if (NameTagWidgetFinder.Class)
	{
		OverHeadWidgetComponent->SetWidgetClass(NameTagWidgetFinder.Class);
		OverHeadWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		OverHeadWidgetComponent->SetDrawSize(FVector2D(400.0f, 50.0f));
		OverHeadWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 플레이어 HP, 타이머, 차징 바가 표시되는 HUD 위젯 설정
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetFinder
	(TEXT("/Game/UI/WBP_HUDWidget.WBP_HUDWidget_C"));
	if (HUDWidgetFinder.Class)
	{
		HUDWidgetClass = HUDWidgetFinder.Class;
	}
}


void AEPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// 폭탄 오브젝트 풀을 관리하는 매니저 초기화
	if (HasAuthority())
	{
		BombManager->MakeBombObjectPool(BP_Bomb);
		AEPDeathMatchGameMode* GameMode = Cast<AEPDeathMatchGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			StatComponent->OnHpZeroThreeParams.AddUObject(GameMode, &AEPDeathMatchGameMode::OnPlayerKilled);
		}
	}

	// 플레이어 캐릭터 상단에 표시될 UI 초기화
	OverHeadWidget = Cast<UEPOverHeadWidget>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (OverHeadWidget)
	{
		// PlayerState가 생성되기 전에는 이름이 표시되지 않으므로 2초 뒤에 이름을 표시하도록 타이머 설정
		FTimerHandle CharacterPlayerBeginPlayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(CharacterPlayerBeginPlayTimerHandle, this, &AEPCharacterBase::SetOverHeadPlayerNameUI, 2.0f, false, -1.0f);
		
		// 로컬 플레이어인 경우에는 오버헤드 위젯을 숨김(본인 체력은 HUD에 표시)
		if (IsLocallyControlled()) 
		{
			OverHeadWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			OverHeadWidget->UpdateHpBar(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
			StatComponent->OnHpChanged.AddUObject(OverHeadWidget, &UEPOverHeadWidget::UpdateHpBar);
		}
	}

	// HUD 위젯 초기화
	if (IsLocallyControlled() && HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UEPHUDWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport(10);
			HUDWidget->UpdateHpBar(StatComponent->GetCurrentHp(), StatComponent->GetMaxHp());
			StatComponent->OnHpChanged.AddUObject(HUDWidget, &UEPHUDWidget::UpdateHpBar);
		}
	}

	// 캐릭터의 모든 준비가 완료되었음을 서버에 알림
	if (IsLocallyControlled())
	{
		AEPPlayerController* EPPlayerController = Cast<AEPPlayerController>(GetController());
		if (EPPlayerController)
		{
			EPPlayerController->OnStartMainGame.AddUObject(this, &AEPCharacterBase::StartMainGame);
		}
		ServerRPCPlayerReady();
	}

	// 게임이 시작되기 전에는 입력 비활성화 유지
	DisableInput(GetWorld()->GetFirstPlayerController());
}

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


float AEPCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	AController* EventCauser = GetController();
	StatComponent->TakeDamage(Damage, EventCauser, EventInstigator, DamageCauser);

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

	if (OverHeadWidget)
	{
		OverHeadWidget->UpdateNameTag(PlayerName);
	}
}

void AEPCharacterBase::ServerRPCPlayerReady_Implementation()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		AEPDeathMatchGameMode* EPGameMode = Cast<AEPDeathMatchGameMode>(GameMode);
		if (EPGameMode)
		{
			EPGameMode->CheckAllPlayersReady();
		}
	}
}


