// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RASProjectile.generated.h"

UCLASS()
class PROJECTRAS_API ARASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARASProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> ProjectileBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	float HpDamege = 0;
	float StaminaDamage = 0;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*OverlappedComponent: 이벤트를 발생시킨 컴포넌트(현재 객체의 컴포넌트).
	OtherActor: 충돌한 다른 액터.
	OtherComp : OtherActor에서의 충돌한 컴포넌트.
	OtherBodyIndex : 충돌한 다른 액터의 본체 인덱스(본체가 여러 개인 경우).
	bFromSweep : 스위프트 충돌 여부(충돌을 스위프트로 검출했는지 여부).
	SweepResult : 충돌 정보를 제공하는 FHitResult 구조체.*/
	UFUNCTION()
	void OnHitAnyWhere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void DestroyProjectile();

	void FireProjectile(class AActor* InOwner, FVector TartgetLocation, int32 InDamage = 0, int32 InStanimaDamage = 0);
};
