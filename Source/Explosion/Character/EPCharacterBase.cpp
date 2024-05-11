// Fill out your copyright notice in the Description page of Project Settings.


#include "EPCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Explosion/UI/EPNameTagWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEPCharacterBase::AEPCharacterBase()
{
	// 컨트롤러 회전
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 0.2f;

	// Mesh 설정
	GetMesh() ->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// NameTag 설정
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetMesh());
	WidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));

	static ConstructorHelpers::FClassFinder<UEPNameTagWidget> NameTagWidgetFinder(TEXT("/Game/UI/WBP_NameTagWidget.WBP_NameTagWidget_C"));
	if (NameTagWidgetFinder.Class)
	{
		WidgetComponent->SetWidgetClass(NameTagWidgetFinder.Class);
		WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComponent->SetDrawSize(FVector2D(200.0f, 50.0f));
		WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void AEPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//OnThrowingBombDelegate.AddUObject(this, &AEPCharacterBase::OnThrowingBomb);

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

void AEPCharacterBase::OnThrowingBomb()
{
}

void AEPCharacterBase::OnReloadingBomb()
{
}


