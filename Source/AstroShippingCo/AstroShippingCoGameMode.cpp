// Copyright Epic Games, Inc. All Rights Reserved. test


#include "AstroShippingCoGameMode.h"
#include "AstroShippingCoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAstroShippingCoGameMode::AAstroShippingCoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerController/BP_PlayerController.BP_PlayerController_C"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}
