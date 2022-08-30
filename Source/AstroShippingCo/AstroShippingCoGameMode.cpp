// Copyright Epic Games, Inc. All Rights Reserved. test

#include "AstroShippingCoGameMode.h"
#include "AstroShippingCoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAstroShippingCoGameMode::AAstroShippingCoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_PlayerController"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAstroShippingCoGameMode::BeginPlay()
{
    Super::BeginPlay();

    //Bind our Player died delegate to the Gamemode's PlayerDied function.
    if (!OnPlayerDied.IsBound())
    {
        OnPlayerDied.AddDynamic(this, &AAstroShippingCoGameMode::PlayerDied);
    }

}

void AAstroShippingCoGameMode::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);
}

void AAstroShippingCoGameMode::PlayerDied(ACharacter* Character)
{
    //Get a reference to our Character's Player Controller
    AController* CharacterController = Character->GetController();
    RestartPlayer(CharacterController);
}
