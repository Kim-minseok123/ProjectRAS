// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/RASStatComponent.h"

// Sets default values for this component's properties
URASStatComponent::URASStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void URASStatComponent::ApplyDamage(float InDamageAmount)
{

}

// Called when the game starts
void URASStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URASStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

