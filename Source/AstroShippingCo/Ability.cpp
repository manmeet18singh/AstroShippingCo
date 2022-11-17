// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

// Sets default values
AAbility::AAbility()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilityMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	AbilityMesh->SetupAttachment(RootComponent);

	AbilityID = 0;
	Equipped_PlayRate = 3.5f;
	Falling_PlayRate = 3.5f;
}

// Called when the game starts or when spawned
void AAbility::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbility::AbilityPressed_Implementation()
{
	GLog->Log("ABILITY BASE: USE() You shouldn't be calling this function!");
}

void AAbility::AbilityReleased_Implementation()
{
	GLog->Log("ABILITY BASE: USE() You shouldn't be calling this function!");
}

UTexture2D* AAbility::GetThumbnail()
{
	if (AbilityIcon != NULL)
	{
		return AbilityIcon;
	}
	else return nullptr;
}

