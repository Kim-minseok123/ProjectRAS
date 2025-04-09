// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Dash.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/RASBattleInterface.h"
#include "AIController.h"
#include "Utils/RASBlackBoardKey.h"
#include "Character/RASCharacterBase.h"
#include "Character/Monster/Common/RASCommonMonster.h"
#include "Interface/RASBattleInterface.h"

UBTTask_Dash::UBTTask_Dash()
{

}

EBTNodeResult::Type UBTTask_Dash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
    if (!ControllingPawn)
        return EBTNodeResult::Failed;

    IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(ControllingPawn);
    if (!BattleInterface)
        return EBTNodeResult::Failed;

    ARASCharacterBase * Target = Cast<ARASCharacterBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBTarget));
    if (!Target)
        return EBTNodeResult::Failed;

    ARASCommonMonster* Monster = Cast<ARASCommonMonster>(ControllingPawn);
    if (!Monster)
        return EBTNodeResult::Failed;

    FVector MyLocation = Monster->GetActorLocation();
    FVector TargetLocation = Target->GetActorLocation();

    float RandomDistance = FMath::FRandRange(400.0f, 500.0f);

    FVector RandomOffset;
    float RandomAngle;
    float RadianAngle;
    FVector Destination;
    float AngleBetween;

    do
    {
        RandomAngle = FMath::FRandRange(0.0f, 360.0f);
        RadianAngle = FMath::DegreesToRadians(RandomAngle);

        RandomOffset = FVector(FMath::Cos(RadianAngle) * RandomDistance, FMath::Sin(RadianAngle) * RandomDistance, 0.0f);

        Destination = MyLocation + RandomOffset;

        FVector TargetDirection = (TargetLocation - MyLocation).GetSafeNormal();

        AngleBetween = FMath::Acos(FVector::DotProduct(RandomOffset.GetSafeNormal(), TargetDirection)) * (180.0f / PI);
    } while (AngleBetween <= 20.0f);

    FVector Direction = Destination - Monster->GetActorLocation();
    Monster->Dash(Direction, DashSpeed);

    return EBTNodeResult::Succeeded;
}
