#include "Component/Player/ComboAttackComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Animation/AnimInstance.h"
#include "Data/ComboAttackData.h"
#include "TimerManager.h"

UComboAttackComponent::UComboAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentState = NAME_None;
	bHasPendingInput = false;

	bCanAcceptInput = true;
}

void UComboAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// Data Asset이 할당되어 있다면, 에디터에서 구성한 콤보 상태를 ComboStateMap에 복사
	if (ComboDataAsset)
	{
		ComboStateMap.Empty();
		for (const FComboState& State : ComboDataAsset->ComboStates)
		{
			ComboStateMap.Add(State.StateName, State);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ComboDataAsset이 할당되지 않았습니다!"));
	}
}

void UComboAttackComponent::PressComboAction(EAttackType InAttackType)
{
	// 입력 제한이 활성화 중이면 입력 무시
	if (!bCanAcceptInput)
	{
		return;
	}

	// 만약 콤보 진행 중이 아니라면 시작
	if (CurrentState == NAME_None)
	{
		if (InAttackType == EAttackType::LeftClick)
		{
			CurrentState = FName("A");
			StartCombo();

		}
		else if (InAttackType == EAttackType::Shift)
		{
			CurrentState = FName("F");
			StartCombo();
		}
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
			AnimInstance->Montage_JumpToSection(CurrentState, ComboAttackMontage);
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
			FTimerDelegate TimerDel;
			TimerDel.BindUObject(this, &UComboAttackComponent::EndCombo, true);
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, TimerDel, EffectiveTime, false);
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
		// 배열을 순회하여 해당 공격 타입과 일치하는 전이를 찾음
		for (const FComboTransition& Transition : ComboState.Transitions)
		{
			if (Transition.AttackType == InAttackType)
			{
				OutNextState = Transition.NextState;
				return true;
			}
		}
	}
	return false;
}

void UComboAttackComponent::EndCombo(bool InbSetTiemr /*= true*/)
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

	bCanAcceptInput = false;
	if (InbSetTiemr)
	{
		GetWorld()->GetTimerManager().SetTimer(
			ComboResetHandle,
			FTimerDelegate::CreateLambda([this]()
				{
					bCanAcceptInput = true;
				}),
			.7f,
			false
		);
	}
	else
	{
		bCanAcceptInput = true;
	}
}



