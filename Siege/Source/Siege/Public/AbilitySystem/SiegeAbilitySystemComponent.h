// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "SiegeAbilitySystemComponent.generated.h"



/**
 * A couple of add-ons to the AbilitySystemComponent
 */
UCLASS()
class SIEGE_API USiegeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Constructors and overrides
	USiegeAbilitySystemComponent();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UGameplayAbility*>& ActiveAbilities);

	/** Returns the default level used for ability activations, derived from the player state */
	int32 GetDefaultAbilityLevel() const;

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static USiegeAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	FGameplayAbilitySpecHandle GrantAbility(AActor* OwningActor, TSubclassOf<class UGameplayAbility> NewAbility);
	void RemoveAbility(AActor* OwningActor, const FGameplayAbilitySpecHandle& Handle);

};
