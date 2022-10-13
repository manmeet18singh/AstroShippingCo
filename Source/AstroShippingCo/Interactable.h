// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class ASTROSHIPPINGCO_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Unreal Function
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Interact();

	// CPP Function
	virtual void Interact_Implementation();

	// We assume all interactables will have some static mesh
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		UStaticMeshComponent* InteractableMesh;

	// We assume all interactables will have some help text
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		FString InteractableHelpText;
};
