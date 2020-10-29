// Fill out your copyright notice in the Description page of Project Settings.


#include "SiegeAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"
#include "SiegePlayerState.h"

#include "Kismet/KismetSystemLibrary.h"

USiegeAbilitySystemComponent::USiegeAbilitySystemComponent()
{

}

void USiegeAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UGameplayAbility>(ActiveAbility));
		}
	}
}

int32 USiegeAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	if (OwnerActor)
	{
		return Cast<ASiegePlayerState>(OwnerActor)->GetCurrentLevel();
	}
	return 1;
}

USiegeAbilitySystemComponent* USiegeAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<USiegeAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

FGameplayAbilitySpecHandle USiegeAbilitySystemComponent::GrantAbility(AActor* OwningActor, TSubclassOf<class UGameplayAbility> NewAbility)
{
	if (OwningActor->GetLocalRole() == ROLE_Authority)
	{
		return GiveAbility(FGameplayAbilitySpec(NewAbility.GetDefaultObject(), 1));
	}
	
	return FGameplayAbilitySpecHandle();
}

void USiegeAbilitySystemComponent::RemoveAbility(AActor* OwningActor, const FGameplayAbilitySpecHandle& Handle)
{
	if (OwningActor->HasAuthority())
	{
		ClearAbility(Handle);
	}
}
