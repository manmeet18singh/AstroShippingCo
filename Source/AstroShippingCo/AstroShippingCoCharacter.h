// Copyright Epic Games, Inc. All Rights Reserved. test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interactable.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Health/AC_HealthComponent.h"
#include "AstroShippingCoCharacter.generated.h"

UCLASS(config=Game)
class AAstroShippingCoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	 AAstroShippingCoCharacter();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* InteractMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* UnequipMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
		UAC_HealthComponent* HealthComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;

	UFUNCTION(BlueprintPure)
		float GetSpeed();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
		float GravityMultiplier = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
		bool isJumping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
		bool isFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ability)
		bool isCurrentlyEquipped;

	//// Get armed status
	//UFUNCTION(BlueprintPure)	
	//	bool GetIsEquipped();

	// The player's inventory slot used to get current equipped
	APickup* CurrentEquip;

	//// The player's inventory slot used to get current equipped
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//	TSubclassOf<class AAbility> CurrentAbility;

	//void SetCurrentAbility(TSubclassOf<class AAbility> Ability);

	//// Set current equip
	//void SetCurrentEquip(APickup* Item);

	// Player Help Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		FString HelpText;

	// The batteries the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int32 BatteryCount;

	// The Packing Peanuts the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int32 PeanutCount;

	/* Update the batteries
	@param Amount this is the amount to update the batteries by, can be pos or neg
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UpdateBattery(int32 Amount);

	/* Update the peanuts
@param Amount this is the amount to update the peanuts by, can be pos or neg
*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UpdatePeanut(int32 Amount);

	/*Adds an item to the inventory*/
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		bool AddItemToInventory(APickup* Item);

	/*Gets Thumbnail for a given inventory slot (for inventory hud)*/
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		UTexture2D* GetThumbnailAtInventorySlot(int32 Slot);

	// Get Item name for given inventory slot
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
		FString GetItemNameAtInventorySlot(int32 Slot);

	// Uses the item at a given inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void EquipItemAtInventorySlot(int32 Slot);

	// Unequips at a given inventory slot
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
		void UnequipAtInventorySlot(int32 Slot);

protected:

	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void PlayerJump();
	void PlayerStopJump();

	virtual void Landed(const FHitResult& Hit) override;

	void GravityMultiplierTimer();
	void FallCheckTimer();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

		//Called when our Actor is destroyed during Gameplay.
	virtual void Destroyed();

	//Call Gamemode class to Restart Player Character.
	void CallRestartPlayer();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FTimerHandle GravityMultiplierTimerHandle;
	FTimerHandle FallCheckTimerHandle;

private:

	// The player's interact reach
	float Reach;

	// Interacts with current interactable, provided there is one
	void Interact();

	// Checks for interactable items directly infront of player using line trace, called on per tick basis
	void CheckForInteractables();

	// Interactable Player is looking at
	AInteractable* CurrentInteractable;

	// The player's inventory, represented as a tarray of pickup objects
	UPROPERTY(VisibleAnywhere, Category = Character)
		TArray<APickup*>Inventory;

};

