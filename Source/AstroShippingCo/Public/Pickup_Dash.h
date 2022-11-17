// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Pickup.h"
#include "Pickup_Dash.generated.h"

/**
 * 
 */
UCLASS()
class ASTROSHIPPINGCO_API APickup_Dash : public APickup
{
	GENERATED_BODY()
	
	APickup_Dash();

	virtual void Equip_Implementation() override;

	virtual void Unequip_Implementation() override;

};
