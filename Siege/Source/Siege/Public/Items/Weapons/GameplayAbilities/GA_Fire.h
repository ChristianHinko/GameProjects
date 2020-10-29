// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiegeGameplayAbility.h"

#include "GA_Fire.generated.h"



/**
 * Calls "Fire" on current weapon through the ability system
 */
UCLASS()
class SIEGE_API UGA_Fire : public USiegeGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
		void OnTaskCompleted();
	UFUNCTION()
		void OnTaskFailed();

};
