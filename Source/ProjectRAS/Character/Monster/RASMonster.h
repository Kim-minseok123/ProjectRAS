// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RASCharacterBase.h"
#include "RASMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRAS_API ARASMonster : public ARASCharacterBase
{
	GENERATED_BODY()
public:
	ARASMonster();

	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage; 
};
