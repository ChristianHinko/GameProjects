// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_SwitchWeapon.h"

#include "GAT_SwitchWeapon.h"
#include "SiegeCharacter.h"
#include "WeaponInventory.h"


bool UGA_SwitchWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	//////////
	ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(ActorInfo->AvatarActor);
	if (SiegeCharacter && SiegeCharacter->GetWeaponInventoryComponent())
	{
		UWeaponInventory* WeaponInventoryComponent = SiegeCharacter->GetWeaponInventoryComponent();


		if (WeaponInventoryComponent->GetWeaponInventory().Num() < 2)
		{
			return false;
		}
	}

	//////////
	return true;
}
void UGA_SwitchWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(ActorInfo->AvatarActor);
		if (SiegeCharacter)
		{
			UGAT_SwitchWeapon* SwitchWeaponTask = UGAT_SwitchWeapon::CreateSwitchWeaponTask(this, FName("SwitchWeaponProxy"), SiegeCharacter);
			SwitchWeaponTask->OnCompleted.AddUObject(this, &UGA_SwitchWeapon::OnTaskCompleted);
			SwitchWeaponTask->OnFailed.AddUObject(this, &UGA_SwitchWeapon::OnTaskFailed);
			SwitchWeaponTask->ReadyForActivation();
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

void UGA_SwitchWeapon::OnTaskCompleted()
{
			

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
void UGA_SwitchWeapon::OnTaskFailed()
{


	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
