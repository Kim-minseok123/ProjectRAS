// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Projectile/RASProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/RASBattleInterface.h"
#include "Character/RASCharacterBase.h"

// Sets default values
ARASProjectile::ARASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	ProjectileBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileBody"));
	ProjectileBody->SetupAttachment(SceneRoot);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ArrowMovement"));
	ProjectileMovement->SetUpdatedComponent(ProjectileBody);
	ProjectileMovement->InitialSpeed = 3000.f;  
	ProjectileMovement->MaxSpeed = 3000.f;      
	ProjectileMovement->bRotationFollowsVelocity = false; 
	ProjectileMovement->bShouldBounce = false;  
	ProjectileMovement->ProjectileGravityScale = 0.0f; 

	ProjectileBody->SetGenerateOverlapEvents(true);
	ProjectileBody->OnComponentBeginOverlap.AddDynamic(this, &ARASProjectile::OnHitAnyWhere);

	HpDamege = 0;
	StaminaDamage = 0;

	InitialLifeSpan = 5.f;
}

// Called when the game starts or when spawned
void ARASProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARASProjectile::OnHitAnyWhere(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Owner && OtherActor != this && OtherActor != Owner && OtherComp)
	{
		IRASBattleInterface* BattleInterface = Cast<IRASBattleInterface>(OtherActor);
		if (BattleInterface == nullptr) return;
		
		ARASCharacterBase* OwnerCharacter = Cast<ARASCharacterBase>(Owner);
		if (Owner == nullptr) return;
		BattleInterface->HitFromActor(OwnerCharacter, HpDamege, StaminaDamage);

		DestroyProjectile();
	}
}

void ARASProjectile::DestroyProjectile()
{
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->SetComponentTickEnabled(false);
	ProjectileBody->SetVisibility(false);
	ProjectileBody->SetGenerateOverlapEvents(false);
	ProjectileBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Destroy();
}

void ARASProjectile::FireProjectile(class AActor* InOwner, FVector TartgetLocation, int32 InDamage /*= 0*/, int32 InStanimaDamage /*= 0*/)
{
	if (ProjectileMovement)
	{
		FVector Direction = TartgetLocation.GetSafeNormal();

		ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
		SetOwner(InOwner);
		HpDamege = InDamage;
		StaminaDamage = InStanimaDamage;
	}
}


