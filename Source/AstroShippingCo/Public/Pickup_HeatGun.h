// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Pickup.h"
#include "Pickup_HeatGun.generated.h"

/**
 * 
 */
UCLASS()
class ASTROSHIPPINGCO_API APickup_HeatGun : public APickup
{
	GENERATED_BODY()

public:
	APickup_HeatGun();

	virtual void Equip_Implementation() override;

	virtual void Unequip_Implementation() override;

};
