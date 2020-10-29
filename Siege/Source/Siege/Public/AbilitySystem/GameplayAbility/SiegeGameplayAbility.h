// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SiegeGameplayAbility.generated.h"



/**
 * All Gameplay Abilities in this project inherit from this
 */
UCLASS()
class SIEGE_API USiegeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	USiegeGameplayAbility();

};
