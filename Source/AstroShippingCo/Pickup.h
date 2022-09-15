// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/ShapeComponent.h"
#include "Components/TimelineComponent.h"
#include "Pickup.generated.h"

/**
 * 
 */

class UCurveFloat;

UCLASS()
class ASTROSHIPPINGCO_API APickup : public AInteractable
{
	GENERATED_BODY()
	
public:
	APickup();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	virtual void Tick(float DeltaTime) override;

	// Unreal Function
	UFUNCTION(BlueprintNativeEvent)
		void Equip();

	// Unreal Function
	UFUNCTION(BlueprintNativeEvent)
		void Use();

	// Unreal Function
	UFUNCTION(BlueprintNativeEvent)
		void Unequip();

	// CPP Function
	virtual void Equip_Implementation();

	virtual void Unequip_Implementation();

	virtual void Use_Implementation();

	// Assume most pickups will have some icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Properties")
		UTexture2D* PickupIcon;

	// Assume most pickups will have some name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Properties")
		FString PickupName;

	// Assume most pickups will have some value (damage, currency, health)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Properties")
		int32 PickupValue;

	// Assume pickup will happen with collision
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		UShapeComponent* PickUpCollider;

	// Is this an equipable item? 
	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
		bool bIsEquippable;

	// Is the pickup already in the world?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Properties")
		bool bIsEquipped;

	UFUNCTION()
		void TimelineProgress(float Value);

	// This is called when the item is picked up and needs to be cleaned up from the level
	void OnPickedUp();

	// This is called to check if item is equippable
	bool IsEquippable();

	// This is called to check if item is in the world
	bool IsEquipped();

	FTimeline CurveTimeline;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UCurveFloat* Floating_CurveFloat;

	UPROPERTY()
		FVector Floating_StartLoc;

	UPROPERTY()
		FVector Floating_EndLoc;

	UPROPERTY(EditAnywhere, Category = "Animation")
		float Floating_ZOffset;

	UPROPERTY(EditAnywhere, Category = "Animation")
		float Rotating_YawValue;
};
