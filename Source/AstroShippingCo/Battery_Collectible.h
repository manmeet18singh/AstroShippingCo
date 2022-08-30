// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h" // Switch level, probably temporary
#include "Battery_Collectible.generated.h"

UCLASS()
class ASTROSHIPPINGCO_API ABattery_Collectible : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattery_Collectible();

	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Called every frame
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* BatteryRoot;

	//The static mesh for the battery
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BatteryMesh;

	//Collider Box
	UPROPERTY(EditAnywhere)
	UShapeComponent* BatteryBox;

	//Rotation Rate of PickUp
	UPROPERTY(EditAnywhere)
	FRotator RotationRate;

	// Materials
	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* MaterialOne;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterial* Material;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInstance* MaterialInstance;

	//UFUNCTION()
	//void OnPlayerEnterBatteryBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex, bool bFromSweep, const FHitResult& SweeepResult);

protected:

	// Count destroyed batteries
	int destroyed = 0;
};