// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/RASBattleInterface.h"
#include "RASCharacterBase.generated.h"

UCLASS()
class PROJECTRAS_API ARASCharacterBase : public ACharacter, public IRASBattleInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARASCharacterBase();

	virtual void SetAttackFinishedDelegate(const FCharacterAttackFinished& InOnAttackFinished) override;

	virtual void StartAttackMontage(int InAttackNumber = 0) override;
	
	virtual void HitFromActor(class ARASCharacterBase* InFrom, int InDamage) override;

	virtual void EndAttack() override;

	virtual void SetVisibleIndicator(bool InbIsVisible) override
	{

	}

	FString& GetCreatureName() { return CreatureName; }

	virtual void KnockbackToDirection(class AActor* InFrom, FVector Direction, float InPower) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats)
	TObjectPtr<class URASStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	FString CreatureName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	float KnockbackFigure = 30;
};
