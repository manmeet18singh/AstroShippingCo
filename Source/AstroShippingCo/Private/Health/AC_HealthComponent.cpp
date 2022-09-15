// Fill out your copyright notice in the Description page of Project Settings.


#include "Health/AC_HealthComponent.h"

 //Sets default values for this component's properties
UAC_HealthComponent::UAC_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; //true;

	maxHealth = 100;
	currentHealth = maxHealth;
}

void UAC_HealthComponent::AddHealth(int value)
{
	if (currentHealth != maxHealth)
	{
		currentHealth += value;
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HEALTH IS FULL!"));
	}
}


 //Called when the game starts
void UAC_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get reference to owning actor/pawn
	AActor* owner = GetOwner();

	if (owner)
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UAC_HealthComponent::TakeDamage);
	}
}

void UAC_HealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}
		
	//currentHealth -= Damage;
	currentHealth = FMath::Clamp(currentHealth - Damage, 0.0f, maxHealth);
}