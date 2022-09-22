// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability.generated.h"

UCLASS()
class ASTROSHIPPINGCO_API AAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbility();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Unreal Function 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void AbilityPressed();

	// Unreal Function 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void AbilityReleased();

	// Cpp Function
	virtual void AbilityPressed_Implementation();

	// Cpp Function
	virtual void AbilityReleased_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties", meta = (AllowPrivateAccess = "true"))
		int AbilityID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* AbilityMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
		UAnimSequence* Anim_Equipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties", meta = (AllowPrivateAccess = "true"))
		float Equipped_PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
		UAnimSequence* Anim_Falling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties", meta = (AllowPrivateAccess = "true"))
		float Falling_PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
		UAnimMontage* Mon_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Properties")
		UAnimMontage* Mon_B;

};
