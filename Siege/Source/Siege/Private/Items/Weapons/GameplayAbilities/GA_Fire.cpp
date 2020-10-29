// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Fire.h"

#include "GAT_Fire.h"
#include "SiegeCharacter.h"
#include "WeaponInventory.h"
#include "BaseWeapon.h"



bool UGA_Fire::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	//////////



	//////////
	return true;
}
void UGA_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(ActorInfo->AvatarActor);
		if (SiegeCharacter)
		{
			UGAT_Fire* FireTask = UGAT_Fire::CreateFireTask(this, FName("FireProxy"), SiegeCharacter);
			FireTask->OnCompleted.AddUObject(this, &UGA_Fire::OnTaskCompleted);
			FireTask->OnFailed.AddUObject(this, &UGA_Fire::OnTaskFailed);
			FireTask->ReadyForActivation();
		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGA_Fire::OnTaskCompleted()
{
	

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
void UGA_Fire::OnTaskFailed()
{
	

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
