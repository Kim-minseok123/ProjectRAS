// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_FindTarget.h"
#include "AIController.h"
#include "Interface/Monster/RASMonsterInfoInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Utils/RASCollisionChannels.h"
#include "Utils/RASBlackBoardKey.h"
#include "Character/RASCharacterBase.h"
#include "Character/Player/RASPlayer.h"
#include "Component/Player/RASCombatComponent.h"

UBTService_FindTarget::UBTService_FindTarget()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	IRASMonsterInfoInterface* MonsterInfo = Cast<IRASMonsterInfoInterface>(ControllingPawn);
	if (MonsterInfo == nullptr)
		return;

	ARASCharacterBase* Target = MonsterInfo->GetTarget();
	if (Target != nullptr)
	{
		if (ARASPlayer* Player = Cast<ARASPlayer>(Target))
		{
			if (Player->GetCombatComponent()->GetCombatState() == EPlayerCombatState::Deathing)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBTarget, nullptr);
				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBTarget, Target);
		return;
	}
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			ARASCharacterBase* Pawn = Cast<ARASCharacterBase>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				FVector MonsterToPlayer = Pawn->GetActorLocation() - ControllingPawn->GetActorLocation();
				MonsterToPlayer.Normalize();

				FVector ForwardVector = ControllingPawn->GetActorForwardVector();
				ForwardVector.Normalize();

				float DotResult = MonsterToPlayer.Dot(ForwardVector);
				if (DotResult > 0)
				{
					MonsterInfo->SetTarget(Pawn);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBTarget, Pawn);
				}
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBTarget, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
