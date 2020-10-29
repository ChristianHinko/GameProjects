// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SiegePlayerController.generated.h"


class ASiegeCharacter;
class ASiegePlayerState;



/**
 * Ability-system-aware player controller
 */
UCLASS()
class SIEGE_API ASiegePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	/**
	 * Whether it's possessing a new character from a new current FCharacterInfo or whether the character needs to be updated on it's variables (ie its attribute set)
	 * this is the function that gets called
	 */
	UFUNCTION()
		void UpdateCharacterFromInfo();

protected:
		// EVENTS
	virtual void BeginPlay() override;
	
	virtual void OnPossess(class APawn* Pawn) override;
	
	virtual void AcknowledgePossession(class APawn* Pawn) override;
		//
};
