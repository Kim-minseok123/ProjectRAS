// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Animation/RASAnimationComponent.h"
#include "RASMonsterAnimComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API URASMonsterAnimComponent : public URASAnimationComponent
{
	GENERATED_BODY()
	
public:
	URASMonsterAnimComponent();

	virtual TObjectPtr<class UAnimMontage> GetMontageByName(FName MontageName) const override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage; 
};
