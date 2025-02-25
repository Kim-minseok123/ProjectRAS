// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RASCharacterStat.h"
#include "RASStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FRASCharacterStats& /*BaseStat*/, const FRASCharacterStats& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAS_API URASStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URASStatComponent();

	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FRASCharacterStats BaseStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
	FRASCharacterStats ModifierStat;

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ApplyDamage(float InDamageAmount);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
