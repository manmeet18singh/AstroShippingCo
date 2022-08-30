// Copyright Epic Games, Inc. All Rights Reserved. test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
};

