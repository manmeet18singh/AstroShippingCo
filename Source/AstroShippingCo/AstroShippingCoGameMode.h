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

	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

	//Tries to Spawn the player's pawn.
	virtual void RestartPlayer(AController* NewPlayer) override;

protected:
    virtual void BeginPlay() override;

    //Called when Player character has died.
    UFUNCTION()
        virtual void PlayerDied(ACharacter* Character);

    //Signature to bind delegate. 
    UPROPERTY()
        FOnPlayerDiedSignature OnPlayerDied;
};


