// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_SetWeapon.h"

#include "GAT_SetWeapon.h"
#include "SiegeCharacter.h"
#include "WeaponInventory.h"

#include "Kismet/KismetSystemLibrary.h"



bool UGA_SetWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	//////////



	//////////
	return true;
}
void UGA_SetWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(ActorInfo->AvatarActor);
		if (SiegeCharacter)
		{
			if (SiegeCharacter->GetWeaponInventoryComponent())
			{
				if (SiegeCharacter->GetWeaponInventoryComponent()->GetWeaponInventory().IsValidIndex(SiegeCharacter->weaponSlot))
				{
					UGAT_SetWeapon* SetWeaponTask = UGAT_SetWeapon::CreateSetWeaponTask(this, FName("SetWeaponProxy"), SiegeCharacter, SiegeCharacter->weaponSlot);
					SetWeaponTask->OnCompleted.AddUObject(this, &UGA_SetWeapon::OnTaskCompleted);
					SetWeaponTask->OnFailed.AddUObject(this, &UGA_SetWeapon::OnTaskFailed);
					SetWeaponTask->ReadyForActivation();
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

void UGA_SetWeapon::OnTaskCompleted()
{


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_SetWeapon::OnTaskFailed()
{
	

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
