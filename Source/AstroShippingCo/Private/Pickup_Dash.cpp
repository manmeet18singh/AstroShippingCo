// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Dash.h"
#include "../AstroShippingCoCharacter.h"
#include "Engine/StaticMeshActor.h"

APickup_Dash::APickup_Dash()
{
	bIsEquippable = true;
}


void APickup_Dash::Equip_Implementation()
{

	AAstroShippingCoCharacter* Character = Cast<AAstroShippingCoCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if (ToSpawn)
	{
		UWorld* world = Character->GetWorld();
		if (world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = Character;

			const FTransform socketTransform = Character->GetMesh()->GetSocketTransform(FName("DashSocket"), ERelativeTransformSpace::RTS_World);

			AActor* spawnedAbility = world->SpawnActor(ToSpawn, &socketTransform);

			if (spawnedAbility == nullptr) return;

			spawnedAbility->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("DashSocket"));
		}

	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("EQUIPPED HEAT GUN")));

	bIsEquipped = true;
}

void APickup_Dash::Unequip_Implementation()
{
	AAstroShippingCoCharacter* Character = Cast<AAstroShippingCoCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Put code here that places the item into the character's inventory
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ToSpawn, FoundActors);
	for (AActor* ActorFound : FoundActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, ActorFound->GetName());
		//Character->AddItemToInventory(this);
		ActorFound->Destroy();
	}

	bIsEquipped = false;
	//Character->SetCurrentEquip(NULL);
	//Character->AddItemToInventory(this);

}
