// Fill out your copyright notice in the Description page of Project Settings.

#include "Battery_Collectible.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABattery_Collectible::ABattery_Collectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BatteryRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BatteryRoot"));
	RootComponent = BatteryRoot;

	BatteryMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("BatteryMesh"));
	BatteryMesh->SetupAttachment(BatteryRoot);

	BatteryBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BatteryBox"));
	BatteryBox->SetupAttachment(BatteryRoot);

	//BatteryBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	//BatteryBox->SetGenerateOverlapEvents(true);
	//BatteryBox->OnComponentBeginOverlap.AddDynamic(this, &ABattery_Collectible::OnPlayerEnterBatteryBox);
	//BatteryBox->AttachToComponent(BatteryRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	this->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
}

// Called when the game starts or when spawned
void ABattery_Collectible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattery_Collectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Rotate the object locally
	this->AddActorLocalRotation(this->RotationRate * DeltaTime);
}

//void ABattery_Collectible::OnPlayerEnterBatteryBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OhterBodyIndex, bool bFromSweep, const FHitResult& SweeepResult)
//{
//	Destroy();
//	destroyed++;
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Battery collected!"));
//
//	// Change to Game Over screen
//	if (destroyed == 1)
//	{
//		UGameplayStatics::OpenLevel(GetWorld(), "GameOverMap", TRAVEL_Absolute);
//	}
//}