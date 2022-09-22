// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Battery.h"
#include "../AstroShippingCoCharacter.h"

APickup_Battery::APickup_Battery()
{
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickUpCollider->SetGenerateOverlapEvents(true);
	PickUpCollider->OnComponentBeginOverlap.AddDynamic(this, &APickup_Battery::OnPlayerEnterPickupCollider);
}

void APickup_Battery::Interact_Implementation()
{
	// Put code here that places the item into the character's inventory

#pragma region Visibility
	//	Use this code here if you just want to change the visibility and not destroy the pickup... (Replace these calls with the DESTROY())

	//	InteractableMesh->SetVisibility(false);
	//	InteractableMesh->SetSimulatePhysics(false);
	//	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//	PickUpCollider->SetVisibility(false);
	//	PickUpCollider->SetSimulatePhysics(false);
	//	PickUpCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
#pragma endregion
	
	AAstroShippingCoCharacter* Character = Cast<AAstroShippingCoCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	
	if (Character->ActorHasTag(TEXT("Zazi"))) {
		Character->UpdateBattery(PickupValue);
		Destroy();
	}
}

void APickup_Battery::OnPlayerEnterPickupCollider(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Zazi"))
	{
		Interact_Implementation();

		if (SoundCue)
		{
			UGameplayStatics::PlaySound2D(OtherActor->GetWorld(), SoundCue);
		}
	}
}

