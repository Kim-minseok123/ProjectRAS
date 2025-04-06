// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/RASMonster.h"
#include "Controller/Monster/RASAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Stat/RASStatComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/RASAimWidget.h"
#include "Component/Player/RASCombatComponent.h"
#include "Component/Monster/RASMonsterAnimComponent.h"

ARASMonster::ARASMonster()
{

	IndicatorWideget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Indicator"));
	IndicatorWideget->SetupAttachment(GetMesh());

	static ConstructorHelpers::FClassFinder<UUserWidget> IndicatorWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/1_ProjectRAS/UI/WBP_Indicator.WBP_Indicator_C'"));
	if (IndicatorWidgetRef.Class)
	{
		IndicatorWideget->SetWidgetClass(IndicatorWidgetRef.Class);
		IndicatorWideget->SetWidgetSpace(EWidgetSpace::Screen);
		IndicatorWideget->SetDrawSize(FVector2D(74, 74));
		IndicatorWideget->SetRelativeLocation(FVector(0, 0, 60));
	}
	MonsterAnimComponent = CreateDefaultSubobject<URASMonsterAnimComponent>(TEXT("MonsterAnimComponent"));

}

void ARASMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(IndicatorWideget == nullptr) return;

	IndicatorWideget->InitWidget();

	URASAimWidget* Widget = Cast<URASAimWidget>(IndicatorWideget->GetUserWidgetObject());
	if(Widget ==nullptr) return;
	Widget->SetupAim();

	Stat->OnStaminaZero.AddUObject(Widget, &URASAimWidget::VisibleLastAim);

	MonsterAnimComponent->SetAnimInstance(GetMesh()->GetAnimInstance());
}

void ARASMonster::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

ARASCharacterBase* ARASMonster::GetTarget()
{
	return Target;
}

void ARASMonster::SetTarget(ARASCharacterBase* InTarget)
{
	Target = InTarget;
}

void ARASMonster::SetAttackFinishedDelegate(const FCharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ARASMonster::StartAttackMontage(int InAttackNumber /*= 0*/)
{

}

void ARASMonster::EndAttack()
{
	OnAttackFinished.ExecuteIfBound();
}

void ARASMonster::HitFromActor(class ARASCharacterBase* InFrom, float InDamage, float InStaminaDamage)
{
	Super::HitFromActor(InFrom, InDamage, InStaminaDamage);

	ARASPlayer* FromPlayer = Cast<ARASPlayer>(InFrom);
	if (FromPlayer == nullptr) return;
	if(FromPlayer->GetCombatComponent()->GetLockedOnTarget() == nullptr)
		FromPlayer->GetCombatComponent()->SetLockedOnTarget(this);
}

void ARASMonster::SetVisibleIndicator(bool InbIsVisible)
{
	if (IndicatorWideget == nullptr) return;
	URASAimWidget* Widget = Cast<URASAimWidget>(IndicatorWideget->GetUserWidgetObject());
	if (Widget == nullptr) return;
	Widget->VisibleAim(InbIsVisible);
}
