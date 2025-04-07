// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RASAnimationComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAS_API URASAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URASAnimationComponent();

	void SetAnimInstance(UAnimInstance* AnimInstance) { MyAnimInstance = AnimInstance; }

	void StopMontage(UAnimMontage* Montage = nullptr, float Time = 0.f);

	void PlayMontageWithSection(UAnimMontage* Montage, FName SectionName, float PlayRate, TFunction<void(UAnimMontage*, bool)> MontageEndedCallback = nullptr);
	
	virtual TObjectPtr<class UAnimMontage> GetMontageByName(FName MontageName) const;

	void ClearAllDelegate();

	void ChangeRootMotionMode(bool bEnable);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim)
	TObjectPtr<class UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Anim)
	TObjectPtr<class UAnimMontage> DeathMontage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TObjectPtr<class UAnimInstance> MyAnimInstance;
};
