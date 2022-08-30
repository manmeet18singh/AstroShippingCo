// Copyright Epic Games, Inc. All Rights Reserved.

#include "AstroShippingCoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AstroShippingCoGameMode.h"

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
}

//////////////////////////////////////////////////////////////////////////
// Input

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

	//Sets up an input key action to call Restart Player.
	PlayerInputComponent->BindAction("Respawn", IE_Pressed, this, &AAstroShippingCoCharacter::CallRestartPlayer);
}

void AAstroShippingCoCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FallCheckTimerHandle, this, &AAstroShippingCoCharacter::FallCheckTimer, 0.1f, true, 0.0f);
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
	Jump();

	GetWorld()->GetTimerManager().SetTimer(GravityMultiplierTimerHandle, this, &AAstroShippingCoCharacter::GravityMultiplierTimer, 0.1f, true, 0.0f);
	isJumping = true;
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

void AAstroShippingCoCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
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
	if ( (Controller != nullptr) && (Value != 0.0f) )
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
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed: %f"), value));

	return GetVelocity().Size();

}

void AAstroShippingCoCharacter::Destroyed()
{
	Super::Destroyed();

	// Example to bind to OnPlayerDied event in GameMode. 
	if (UWorld* World = GetWorld())
	{
		if (AAstroShippingCoGameMode* GameMode = Cast<AAstroShippingCoGameMode>(World->GetAuthGameMode()))
		{
			GameMode->GetOnPlayerDied().Broadcast(this);
		}
	}
}

void AAstroShippingCoCharacter::CallRestartPlayer()
{
	//Get a reference to the Pawn Controller.
	AController* CortollerRef = GetController();

	//Destroy the Player.   
	Destroy();

		//Get the World and GameMode in the world to invoke its restart player function.
		if (UWorld* World = GetWorld())
		{
			if (AAstroShippingCoGameMode* GameMode = Cast<AAstroShippingCoGameMode>(World->GetAuthGameMode()))
			{
				GameMode->RestartPlayer(CortollerRef);
			}
		}
}