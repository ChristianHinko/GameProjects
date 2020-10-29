// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/GameplayAbility/SiegeGameplayAbility.h"

#include "GA_SetWeapon.generated.h"


class ASiegeCharacter;



/**
 * Calls "SetCurrentWeapon in the player's weapon inventory
 */
UCLASS()
class SIEGE_API UGA_SetWeapon : public USiegeGameplayAbility
{
	GENERATED_BODY()

public:
	//uint8 weaponSlot = 0;
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
		void OnTaskCompleted();
	UFUNCTION()
		void OnTaskFailed();
};
