// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Pickup.h"
#include "Pickup_Battery.generated.h"

/**
 * 
 */
UCLASS()
class ASTROSHIPPINGCO_API APickup_Battery : public APickup
{
	GENERATED_BODY()

public:
	APickup_Battery();

	virtual void Interact_Implementation() override;

	UFUNCTION()
		void OnPlayerEnterPickupCollider(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
