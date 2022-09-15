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

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = Character;

	AStaticMeshActor* spawnedPickup = Character->GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), spawnParams);
	spawnedPickup->SetMobility(EComponentMobility::Movable);
	
	spawnedPickup->SetActorRelativeScale3D(FVector(0.7f,0.7f, 0.7f));
	spawnedPickup->SetActorEnableCollision(false);
	
	// does the spawned Pickup have a mesh
	UStaticMeshComponent* MeshComponent = spawnedPickup->GetStaticMeshComponent();
	if (MeshComponent)
	{
		MeshComponent->SetStaticMesh(SpawnMesh);
		//MeshComponent->collision
	}

	spawnedPickup->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("WeaponSocket"));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("EQUIPPED HEAT GUN")));

	bIsEquipped = true;
	Character->SetCurrentEquip(this);
}

void APickup_HeatGun::Unequip_Implementation()
{
	AAstroShippingCoCharacter* Character = Cast<AAstroShippingCoCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Put code here that places the item into the character's inventory
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), FoundActors);
	for (AActor* ActorFound : FoundActors)
	{
		//Character->AddItemToInventory(this);
		if (!ActorFound->ActorHasTag("Level")) {
			ActorFound->Destroy();
		}
	}

	bIsEquipped = false;
	Character->SetCurrentEquip(NULL);
	Character->AddItemToInventory(this);

}

void APickup_HeatGun::Use_Implementation()
{

}
