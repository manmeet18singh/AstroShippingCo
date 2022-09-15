// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Pickup.h"
#include "Pickup_Hover.generated.h"

/**
 * 
 */
UCLASS()
class ASTROSHIPPINGCO_API APickup_Hover : public APickup
{
	GENERATED_BODY()

	APickup_Hover();

	virtual void Equip_Implementation() override;

	virtual void Unequip_Implementation() override;

	virtual void Use_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Properties", meta = (AllowPrivateAccess = "true"))
		UStaticMesh* SpawnMesh;
};
