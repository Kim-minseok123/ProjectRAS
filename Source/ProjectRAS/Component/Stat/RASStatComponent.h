// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RASCharacterStat.h"
#include "RASStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedDelegate, float /*CurrentHp*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRAS_API URASStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	URASStatComponent();

	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStaminaChangedDelegate OnStaminaChanged;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats")
	FRASCharacterStats BaseStats;

    UFUNCTION(BlueprintCallable, Category = "Stats")
	float ApplyDamage(float InDamageAmount);

	void SetHp(float InHp);
	void SetStamina(float InStamina);
	FORCEINLINE const float	GetHp() const						{ return BaseStats.HP; }
	FORCEINLINE const float	GetStamina() const					{ return BaseStats.Stamina; }
	FORCEINLINE const float GetMaxHp() const						{ return BaseStats.MaxHP; }
	FORCEINLINE const float GetMaxStamina() const					{ return BaseStats.MaxStamina; }
	FORCEINLINE void		SetAttackPower(float InAttackPower) { BaseStats.AttackPower = InAttackPower; }
	FORCEINLINE const float	GetAttackPower() const				{ return BaseStats.AttackPower; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
