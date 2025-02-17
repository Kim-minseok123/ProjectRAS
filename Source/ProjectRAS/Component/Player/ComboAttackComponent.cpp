#include "Component/Player/ComboAttackComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

UComboAttackComponent::UComboAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentState = NAME_None;
	bHasPendingInput = false;
}

void UComboAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// 콤보 상태 데이터 초기화
	InitializeComboStates();
}

void UComboAttackComponent::InitializeComboStates()
{
	ComboStateMap.Empty();

	// 상태 A: 콤보 시작 상태 – 입력(좌클릭) 시 B로 전이
	FComboState StateA;
	StateA.StateName = FName("A");
	StateA.EffectiveTime = 0.45f;
	StateA.Transitions.Add(EAttackType::LeftClick, FName("B"));
	StateA.bIsLast = false;
	ComboStateMap.Add(StateA.StateName, StateA);

	// 상태 B: 입력(좌클릭) 시 C, Shift 입력 시 F, F 입력 시 G로 전이
	FComboState StateB;
	StateB.StateName = FName("B");
	StateB.EffectiveTime = 0.55f;
	StateB.Transitions.Add(EAttackType::LeftClick, FName("C"));
	StateB.Transitions.Add(EAttackType::Shift, FName("F"));
	StateB.Transitions.Add(EAttackType::F, FName("G"));
	StateB.bIsLast = false;
	ComboStateMap.Add(StateB.StateName, StateB);

	// 상태 C: 입력(좌클릭) 시 D, Shift 입력 시 E, F 입력 시 H로 전이
	FComboState StateC;
	StateC.StateName = FName("C");
	StateC.EffectiveTime = 0.55f;
	StateC.Transitions.Add(EAttackType::LeftClick, FName("D"));
	StateC.Transitions.Add(EAttackType::Shift, FName("E"));
	StateC.Transitions.Add(EAttackType::F, FName("H"));
	StateC.bIsLast = false;
	ComboStateMap.Add(StateC.StateName, StateC);

	// 터미널 상태: D, E, F, G, H – 더 이상의 입력 없이 콤보 종료
	FComboState StateD;
	StateD.StateName = FName("D");
	StateD.EffectiveTime = 0.3f;
	StateD.bIsLast = false;
	ComboStateMap.Add(StateD.StateName, StateD);

	FComboState StateE;
	StateE.StateName = FName("E");
	StateE.EffectiveTime = 1.2f;
	StateE.bIsLast = false;
	ComboStateMap.Add(StateE.StateName, StateE);

	FComboState StateF;
	StateF.StateName = FName("F");
	StateF.EffectiveTime = .5f;
	StateF.bIsLast = false;
	ComboStateMap.Add(StateF.StateName, StateF);

	FComboState StateG;
	StateG.StateName = FName("G");
	StateG.EffectiveTime = 0.25f;
	StateG.bIsLast = false;
	ComboStateMap.Add(StateG.StateName, StateG);

	FComboState StateH;
	StateH.StateName = FName("H");
	StateH.EffectiveTime = 0.25f;
	StateH.bIsLast = false;
	ComboStateMap.Add(StateH.StateName, StateH);
}

void UComboAttackComponent::PressComboAction(EAttackType InAttackType)
{
	// 만약 콤보 진행 중이 아니라면 시작
	if (CurrentState == NAME_None && InAttackType == EAttackType::LeftClick)
	{
		StartCombo();
		return;
	}

	// 터미널 상태(마지막 공격)가 진행 중이면 새로운 입력은 무시
	if (ComboStateMap.Contains(CurrentState) && ComboStateMap[CurrentState].bIsLast)
	{
		// 마지막 공격 진행 중이므로 새로운 공격 입력을 받지 않음
		return;
	}

	// 콤보 진행 중이며 입력 윈도우 내라면, 입력을 저장
	if (GetWorld()->GetTimerManager().IsTimerActive(ComboTimerHandle))
	{
		bHasPendingInput = true;
		PendingAttackType = InAttackType;
	}
}

void UComboAttackComponent::StartCombo()
{
	// 콤보 시작 시 상태 "A"로 초기화
	CurrentState = FName("A");

	// 플레이어 공격 상태 활성화
	ARASPlayer* Player = Cast<ARASPlayer>(GetOwner());
	if (Player)
	{
		Player->SetIsAttacking(true);

		// 몽타주 재생
		UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
		if (AnimInstance && ComboAttackMontage)
		{
			AnimInstance->Montage_Play(ComboAttackMontage);
		}
	}

	// 현재 상태의 입력 유효 시간에 맞춰 타이머 설정
	SetComboTimer();
}

void UComboAttackComponent::SetComboTimer()
{
	if (ComboStateMap.Contains(CurrentState))
	{
		ARASPlayer* Player = Cast<ARASPlayer>(GetOwner());
		if (Player) if (Player->bIsRolling == true) return;
		float EffectiveTime = ComboStateMap[CurrentState].EffectiveTime;
		if (ComboStateMap[CurrentState].bIsLast == false)
		{
			// 입력 가능한 상태라면 타이머를 설정
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UComboAttackComponent::ComboTimerExpired, EffectiveTime, false);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UComboAttackComponent::EndCombo, EffectiveTime, false);
		}
	}
	else
	{
		EndCombo();
	}
}

void UComboAttackComponent::ComboTimerExpired()
{
	// 입력이 있었다면 상태 전이 시도
	if (bHasPendingInput)
	{
		ARASPlayer* Player = Cast<ARASPlayer>(GetOwner());
		if (Player) if (Player->bIsRolling == true) return;

		FName NextState;
		if (GetNextState(PendingAttackType, NextState))
		{
			CurrentState = NextState;
			bHasPendingInput = false; // 입력 초기화

			// 몽타주에서 해당 섹션으로 점프
			if (Player)
			{
				UAnimInstance* AnimInstance = Player->GetMesh()->GetAnimInstance();
				if (AnimInstance && ComboAttackMontage)
				{
					// 다음 공격으로 전환할 때 Montage_JumpToSection을 호출
					AnimInstance->Montage_Play(ComboAttackMontage);
					AnimInstance->Montage_JumpToSection(CurrentState, ComboAttackMontage);
				}
			}

			// 새 상태에 맞춰 타이머 재설정
			SetComboTimer();
		}
		else
		{
			// 유효한 전이가 없으면 콤보 종료
			EndCombo();
		}
	}
	else
	{
		// 입력이 없으면 콤보 종료
		EndCombo();
	}
}

bool UComboAttackComponent::GetNextState(EAttackType InAttackType, FName& OutNextState)
{
	if (ComboStateMap.Contains(CurrentState))
	{
		const FComboState& ComboState = ComboStateMap[CurrentState];
		if (ComboState.Transitions.Contains(InAttackType))
		{
			OutNextState = ComboState.Transitions[InAttackType];
			return true;
		}
	}
	return false;
}

void UComboAttackComponent::EndCombo()
{
	// 타이머 초기화 및 콤보 상태 리셋
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
	CurrentState = NAME_None;
	bHasPendingInput = false;

	// 플레이어 공격 상태 비활성화
	ARASPlayer* Player = Cast<ARASPlayer>(GetOwner());
	if (Player)
	{
		Player->SetIsAttacking(false);
	}
}


