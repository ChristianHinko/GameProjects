// Fill out your copyright notice in the Description page of Project Settings.


#include "GAT_SwitchWeapon.h"

#include "SiegeCharacter.h"
#include "WeaponInventory.h"



UGAT_SwitchWeapon* UGAT_SwitchWeapon::CreateSwitchWeaponTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, ASiegeCharacter* SiegeCharacter)
{
	UGAT_SwitchWeapon* SwitchWeaponTask = NewAbilityTask<UGAT_SwitchWeapon>(OwningAbility, TaskInstanceName);
	SwitchWeaponTask->SiegeCharacter = SiegeCharacter;
	return SwitchWeaponTask;
}

void UGAT_SwitchWeapon::Activate()
{
	if (!Ability)
	{
		OnFailed.Broadcast();
		return;
	}

	if (SiegeCharacter && SiegeCharacter->GetWeaponInventoryComponent())
	{
		SiegeCharacter->GetWeaponInventoryComponent()->SwitchWeapon();
		OnCompleted.Broadcast();
	}
	else
	{
		OnFailed.Broadcast();
		return;
	}
}
