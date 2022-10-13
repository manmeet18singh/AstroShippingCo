// Copyright Epic Games, Inc. All Rights Reserved.

#include "AstroShippingCoCharacter.h"
#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AstroShippingCoGameMode.h"
#include "Engine/StaticMeshActor.h"

////////////////////////////////////////////////////////////////////////// test
// AAstroShippingCoCharacter

AAstroShippingCoCharacter::AAstroShippingCoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 800.f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create an AC_HealthComponent
	HealthComponent = CreateDefaultSubobject<UAC_HealthComponent>(TEXT("Player Health"));
	AddOwnedComponent(HealthComponent);

	// Load Use Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UnequipMontageObject(TEXT("AnimMontage'/Game/Animations/Zazi/Ability/AM_Zazi_Unequip.AM_Zazi_Unequip'"));
	if (UnequipMontageObject.Succeeded())
	{
		UnequipMontage = UnequipMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EquipMontageObject(TEXT("AnimMontage'/Game/Animations/Zazi/Ability/AM_Zazi_Equip.AM_Zazi_Equip'"));
	if (UnequipMontageObject.Succeeded())
	{
		EquipMontage = EquipMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> InteractMontageObject(TEXT("AnimMontage'/Game/Animations/Zazi/Movement/AM_Zazi_Use.AM_Zazi_Use'"));
	if (InteractMontageObject.Succeeded())
	{
		InteractMontage = InteractMontageObject.Object;
	}

	Reach = 300.0f;
}

void AAstroShippingCoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();
}

void AAstroShippingCoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAstroShippingCoCharacter::PlayerJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AAstroShippingCoCharacter::PlayerStopJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAstroShippingCoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAstroShippingCoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAstroShippingCoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAstroShippingCoCharacter::LookUpAtRate);

	// Setup for interact
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AAstroShippingCoCharacter::Interact);

}

void AAstroShippingCoCharacter::UpdateBattery(int32 Amount)
{
	BatteryCount = BatteryCount + Amount;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PICKED UP BATTERY"));

}

void AAstroShippingCoCharacter::UpdatePeanut(int32 Amount)
{
	PeanutCount = PeanutCount + Amount;
}

bool AAstroShippingCoCharacter::AddItemToInventory(APickup* Item)
{

	if (Item != NULL)
	{
		// Find first slot with nullptr in it
		const int32 AvailableSlot = Inventory.Find(nullptr);

		if (Item->ActorHasTag(FName("PowerUp"))) {
			if (AvailableSlot != INDEX_NONE && !Inventory.Contains(Item))
			{
				Inventory[AvailableSlot] = Item;
				if (Item->SoundCue)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), Item->SoundCue);
				}
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ADEEDDDD TO INNNVENTORRRYY!"));
				return true;
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("You can't carry any more items!"));
				return false;
			}
		}
		else if (Item->ActorHasTag(FName("Battery")))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PICKED UP BATTERY"));
			return true;
		}
		else if (Item->ActorHasTag(FName("Health")))
		{
			Item->Interact_Implementation();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PICKED UP HEALTH"));
			return true;
		}
		else if (Item->ActorHasTag(FName("Push")))
		{
			//Item->Interact_Implementation();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PICKED UP HEALTH"));
			return true;
		}
		else if (Item->ActorHasTag(FName("Throw")))
		{
			//Item->Interact_Implementation();
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("PICKED UP HEALTH"));
			return true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("I Don't know what this item is"));
			return false;
		}

	}
	else return false;
}

UTexture2D* AAstroShippingCoCharacter::GetThumbnailAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickupIcon;
	}
	else return nullptr;
}

FString AAstroShippingCoCharacter::GetItemNameAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		return Inventory[Slot]->PickupName;
	}
	else return FString("None");
}

void AAstroShippingCoCharacter::EquipItemAtInventorySlot(int32 Slot)
{
	if (Inventory[Slot] != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CHANGE EQUIP!!!!!!!!!")));

		if (!Inventory[Slot]->IsEquipped())
		{
			// Current selected ability is not equipped and no ability is already equipped
			if (!isCurrentlyEquipped)
			{
				PlayAnimMontage(EquipMontage, 1.0f, FName("Default"));
				Inventory[Slot]->Equip_Implementation();
				isCurrentlyEquipped = true;
				CurrentEquip = Inventory[Slot];
			}
			// Current selected ability is not equipped but another ability is already equipped 
			else if (isCurrentlyEquipped)
			{
				UnequipAtInventorySlot(Slot);
				PlayAnimMontage(EquipMontage, 1.0f, FName("Default"));
				Inventory[Slot]->Equip_Implementation();
				isCurrentlyEquipped = true;
				CurrentEquip = Inventory[Slot];
			}

			/*Inventory[Slot] = NULL;*/  // Delete the item from the inventory
		}
		// Current selected ability is equipped
		else if (Inventory[Slot]->IsEquipped())
		{
			UnequipAtInventorySlot(Slot);
		}
	}

}

void AAstroShippingCoCharacter::UnequipAtInventorySlot(int32 Slot)
{
	////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CURRENT EQUIP")));
	if (Inventory[Slot] != NULL)
	{
		// FOR MULTIPLE POWERUPS LOOP THRU THE INVENTORY AND CHECK IF IT IS SPAWNED IF IT IS TOGGLE bSPAWNED
		//if (Inventory[Slot]->IsEquippable() && Inventory[Slot]->IsEquipped())
		//{
		if (CurrentEquip != NULL)
		{
			CurrentEquip->Unequip_Implementation();
			PlayAnimMontage(UnequipMontage, 1.0f, FName("Default"));
			CurrentEquip = NULL;
			//characterEquipStatus = false;
		}
		//}

	}

	
}

void AAstroShippingCoCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FallCheckTimerHandle, this, &AAstroShippingCoCharacter::FallCheckTimer, 0.1f, true, 0.0f);
	
	// Set inventory size:
	Inventory.SetNum(4);
	CurrentInteractable = nullptr;

}

void AAstroShippingCoCharacter::TurnAtRate(float Rate)
{

	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAstroShippingCoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


#pragma region Jump Physics

void AAstroShippingCoCharacter::PlayerJump()
{
	if (!isPushing) {
		Jump();

		GetWorld()->GetTimerManager().SetTimer(GravityMultiplierTimerHandle, this, &AAstroShippingCoCharacter::GravityMultiplierTimer, 0.1f, true, 0.0f);
		isJumping = true;
	}
}

void AAstroShippingCoCharacter::PlayerStopJump()
{
	GetCharacterMovement()->GravityScale = 4.0f;
}

void AAstroShippingCoCharacter::Landed(const FHitResult& Hit)
{
	GetCharacterMovement()->GravityScale = 1.0f;
	GetWorld()->GetTimerManager().ClearTimer(GravityMultiplierTimerHandle);

	isJumping = false;
	GravityMultiplier = 0.1f;
}

void AAstroShippingCoCharacter::GravityMultiplierTimer()
{
	if (GetCharacterMovement()->GravityScale < 4.0f && isJumping)
	{
		GetCharacterMovement()->GravityScale += GravityMultiplier;
		GravityMultiplier += 0.4f;
	}
	else if (GetCharacterMovement()->GravityScale >= 4.0f && isJumping)
	{
		GetCharacterMovement()->GravityScale = 4.0f;
	}
}

void AAstroShippingCoCharacter::FallCheckTimer()
{
	if (GetCharacterMovement()->IsFalling() && !isJumping)
	{
		GetCharacterMovement()->GravityScale = 4.0f;
	}
}
#pragma endregion

#pragma region Movement

void AAstroShippingCoCharacter::MoveForward(float Value)
{	
	if ((Controller != nullptr) && (Value != 0.0f) && !isPushing)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAstroShippingCoCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) && !isPushing)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

float AAstroShippingCoCharacter::GetSpeed()
{
	////GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed: %f"), value));

	return GetVelocity().Size();

}

#pragma endregion

#pragma region Interaction

void AAstroShippingCoCharacter::Interact()
{

	if (CurrentInteractable != nullptr)
	{
		PlayAnimMontage(InteractMontage, 1.0f, FName("Default"));
		CurrentInteractable->Interact_Implementation();
	}
}

void AAstroShippingCoCharacter::CheckForInteractables()
{
	// get the start and end traces
	FVector StartTrace = GetCapsuleComponent()->GetComponentLocation();
	FVector EndTrace = (GetCapsuleComponent()->GetForwardVector() * Reach) + StartTrace;

	// Declare a hitresult to store the raycast hit in
	FHitResult HitResult;

	// Initialize the query params - ignore the actor
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);

	// Cast line trace
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldDynamic, CQP);

	// Draw Debug Line
	//DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Orange, false, 2.0f);

	AInteractable* HitObject = Cast<AInteractable>(HitResult.GetActor());

	if (HitObject == NULL)
	{
		HelpText = FString("");
		CurrentInteractable = nullptr;
		return;
	}
	else
	{
		CurrentInteractable = HitObject;
		HelpText = HitObject->InteractableHelpText;
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, GetDebugName(CurrentInteractable));

	}
}

#pragma endregion


