// Fill out your copyright notice in the Description page of Project Settings.


#include "GAT_Fire.h"

#include "SiegeCharacter.h"
#include "WeaponInventory.h"
#include "BaseWeapon.h"



UGAT_Fire* UGAT_Fire::CreateFireTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, ASiegeCharacter* SiegeCharacter)
{
	UGAT_Fire* FireTask = NewAbilityTask<UGAT_Fire>(OwningAbility, TaskInstanceName);
	FireTask->SiegeCharacter = SiegeCharacter;
	return FireTask;
}

void UGAT_Fire::Activate()
{
	if (!Ability)
	{
		OnFailed.Broadcast();
		return;
	}

	if (SiegeCharacter && SiegeCharacter->GetWeaponInventoryComponent() && SiegeCharacter->GetWeaponInventoryComponent()->GetCurrentWeapon())
	{
		SiegeCharacter->GetWeaponInventoryComponent()->GetCurrentWeapon()->Fire();
		OnCompleted.Broadcast();
	}
	else
	{
		OnFailed.Broadcast();
		return;
	}
}
