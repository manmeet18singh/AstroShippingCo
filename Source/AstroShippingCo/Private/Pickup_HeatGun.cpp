// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_HeatGun.h"
#include "../AstroShippingCoCharacter.h"
#include "Engine/StaticMeshActor.h"

APickup_HeatGun::APickup_HeatGun()
{
	bIsEquippable = true;
}

void APickup_HeatGun::Equip_Implementation()
{
	
	AAstroShippingCoCharacter* Character = Cast<AAstroShippingCoCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (ToSpawn)
	{
		UWorld* world = Character->GetWorld();
		if (world)
		{		
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = Character;
		
			const FTransform socketTransform = Character->GetMesh()->GetSocketTransform(FName("WeaponSocket"), ERelativeTransformSpace::RTS_World);

			AActor* spawnedAbility = world->SpawnActor(ToSpawn, &socketTransform);
			
			if (spawnedAbility == nullptr) return;

			spawnedAbility->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));
		}

	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("EQUIPPED HEAT GUN")));

	bIsEquipped = true;
}

void APickup_HeatGun::Unequip_Implementation()
{
	AAstroShippingCoCharacter* Character = Cast<AAstroShippingCoCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Put code here that places the item into the character's inventory
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ToSpawn, FoundActors);
	for (AActor* ActorFound : FoundActors)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, ActorFound->GetName());
		//Character->AddItemToInventory(this);
		ActorFound->Destroy();
	}

	bIsEquipped = false;
	//Character->SetCurrentEquip(NULL);
	//Character->AddItemToInventory(this);

}