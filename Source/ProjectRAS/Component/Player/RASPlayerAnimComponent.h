// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Animation/RASAnimationComponent.h"
#include "RASPlayerAnimComponent.generated.h"


UCLASS()
class PROJECTRAS_API URASPlayerAnimComponent : public URASAnimationComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URASPlayerAnimComponent();

	virtual TObjectPtr<class UAnimMontage> GetMontageByName(FName MontageName) const override;

	bool StopParryingAnimation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SkillMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ParryingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ExecuteMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, Meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> RollMontage;

};
