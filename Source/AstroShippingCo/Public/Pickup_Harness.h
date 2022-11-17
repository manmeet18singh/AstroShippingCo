// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Pickup.h"
#include "Pickup_Harness.generated.h"

/**
 * 
 */
UCLASS()
class ASTROSHIPPINGCO_API APickup_Harness : public APickup
{
	GENERATED_BODY()
	
	APickup_Harness();

	virtual void Equip_Implementation() override;

	virtual void Unequip_Implementation() override;

};
