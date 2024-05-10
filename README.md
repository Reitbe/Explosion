![게임 배너.png](/ReadMeSource/banner.png)

- 태그: 3인칭, 멀티플레이어
- 사용 기술: Github, Unreal Engine 5, VisualStudio
- 팀 구성: 프로그래머 1인 개발
- 플랫폼: PC
- 작업 기간: 2024년 4월 1일 → 작업 중
<br>

## ❗개요
- **Unreal Engine 5를 사용한 PC 기반 3인칭 멀티플레이 슈팅 게임**
- **진행 중인 프로젝트입니다.** 현재 캐릭터 컨트롤과 애니메이션을 완성했으며, 폭탄과 데미지 시스템을 제작 중입니다.
- Unreal Engine 5에서 제공하는 다양한 기능들을 사용해 보는 것이 목표입니다. 가급적 블루프린트가 아닌 C++을 사용하여 콘텐츠를 제작합니다.
<br>

## 📜 주요 기능
- 캐릭터 컨트롤<br>
   Unreal Engine의 향상된 입력을 사용하여 캐릭터의 2D 이동 / 점프 / 던지기 동작이 가능합니다.
    
- 캐릭터 애니메이션 적용<br>
    애니메이션 리타게팅 / AimOffset / 상하체 분리 적용 / 몽타주를 통한 던지기 동작 등을 적용하여 캐릭터 애니메이션을 구성했습니다. 
    
- 폭탄 던지기<br>
    조준 여부에 관계없이 폭탄을 던질 수 있습니다. 조준을 한 시간에 비례하여 최대 2배의 가중치를 얻습니다. 
    

## 🖥️ 개발 내용
### 폭탄 던지기

```cpp
// 폭탄을 던지는 동작
void AEPCharacterPlayer::Throwing()
{
  // 던지는 행동 중복 방지
	if (bIsThrowing) 
	{
		return;
	}
	bIsThrowing = true;

	// 던지는 애니메이션 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(ThrowingMontage);
		
		// 애니메이션 종료시 던지기 기능 재활성화
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AEPCharacterPlayer::Throwing_OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowingMontage);
	}

	// 미조준 상태 - 타이머 작동X
	if (bIsAiming == false) 
	{	
		DamageMultiplier = 1.0f;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("조준 없이 던져"));
	}
	// 조준 상태 - 타이머 작동O
	else
	{
		// 최대 충전(폭탄 개발 이후 적용 예정)
		if ((DamageMultiplier == 2.0f))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("최대 충전"));
		}
		// 시간 비례 충전(폭탄 개발 이후 적용 예정)
		else 
		{
			DamageMultiplier += GetWorldTimerManager().GetTimerElapsed(ChargingRateTimerHandle) / 2.0f;
			FString str = FString::Printf(TEXT("비례충전 - %f"), DamageMultiplier);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, str);
		}
	}
}

// 던지는 동작이 끝났을 때
void AEPCharacterPlayer::Throwing_OnMontageEnded(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsThrowing = false;
	// 조준 중이었을 경우 차징이 자동으로 다시 시작된다
	if (bIsAiming)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AimingMontage);
		}
		
		// 최대 충전시간이 지나도 차징 동작은 유지되지만 값은 오르지 않는다
		DamageMultiplier = 1.0f;
		GetWorldTimerManager().ClearTimer(ChargingRateTimerHandle);
		GetWorldTimerManager().SetTimer(ChargingRateTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				DamageMultiplier = 2.0f;
			}
		), 2.0f, false);
	}
}
```

- 폭탄을 던지는 동작은 조준 여부의 변경이나 던지는 동작의 재입력에 관계없이 진행되어야 합니다. 그렇기에 애니메이션의 시작과 끝에서 상태를 변경하며, 이 상태를 기반으로 동작이 진행됩니다.
- 애니메이션이 끝났을 때, 조준 여부를 판단하여 차징을 다시 실행합니다.
