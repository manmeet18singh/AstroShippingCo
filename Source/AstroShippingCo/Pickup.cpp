// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "AstroShippingCoCharacter.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

APickup::APickup()
{
	// Tick Default
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set up the mesh, name, help text and item value
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	InteractableMesh->SetupAttachment(RootComponent);

	PickUpCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PickUpCollider"));
	PickUpCollider->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	PickUpCollider->SetupAttachment(InteractableMesh);

	PickupName = FString("Enter an pickup name here...");
	InteractableHelpText = FString("Press Use Button");
	PickupValue = 0;
	bIsEquippable = false;
	bIsEquipped = false;
	Rotating_YawValue = 0;



}

void APickup::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("%s: Press Use Button."), *PickupName);

	Super::BeginPlay();
	if (Floating_CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("TimelineProgress"));

		CurveTimeline.AddInterpFloat(Floating_CurveFloat, TimelineProgress);
		CurveTimeline.SetLooping(true);

		Floating_StartLoc = Floating_EndLoc = GetActorLocation();
		Floating_EndLoc.Z += Floating_ZOffset;

		CurveTimeline.PlayFromStart();
	}

}

void APickup::Interact_Implementation()
{
	AAstroShippingCoCharacter* Character = Cast<AAstroShippingCoCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	// Put code here that places the item into the character's inventory
	if (Character->AddItemToInventory(this))
	{
		OnPickedUp();
	}

}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurveTimeline.TickTimeline(DeltaTime);

	FQuat QuatRot = FQuat(FRotator(0.0f, Rotating_YawValue, 0.0f));
	AddActorLocalRotation(QuatRot, false, 0, ETeleportType::None);
}

void APickup:: Equip_Implementation()
{
	GLog->Log("PICKUP BASE: Equip() You shouldn't be calling this function!");	
}

void APickup::Unequip_Implementation()
{
	GLog->Log("PICKUP BASE: Unequip() You shouldn't be calling this function!");
}

void APickup::TimelineProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(Floating_StartLoc, Floating_EndLoc, Value);
	SetActorLocation(NewLocation);
}

void APickup::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetSimulatePhysics(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickUpCollider->SetVisibility(false);
	PickUpCollider->SetSimulatePhysics(false);
	PickUpCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool APickup::IsEquipped()
{
	return bIsEquipped;
}

bool APickup::IsEquippable()
{
	return bIsEquippable;
}
