// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "AstroShippingCo.h"

// Sets default values
AInteractable::AInteractable()
{
	InteractableHelpText = FString("Press Use Button to Interact with item");
}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();	
}

void AInteractable::Interact_Implementation()
{
	GLog->Log("INTERACT BASE: Interact() You shouldn't be calling this function!");
}

