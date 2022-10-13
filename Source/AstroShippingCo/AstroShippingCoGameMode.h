// Copyright Epic Games, Inc. All Rights Reserved. test

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AstroShippingCoGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

UCLASS(minimalapi)

class AAstroShippingCoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AAstroShippingCoGameMode();
};



