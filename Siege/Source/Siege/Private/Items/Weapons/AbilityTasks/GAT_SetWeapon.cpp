// Fill out your copyright notice in the Description page of Project Settings.


#include "GAT_SetWeapon.h"

#include "SiegeCharacter.h"
#include "WeaponInventory.h"



UGAT_SetWeapon* UGAT_SetWeapon::CreateSetWeaponTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, ASiegeCharacter* SiegeCharacter, uint8 weaponSlot)
{
	UGAT_SetWeapon* SetWeaponTask = NewAbilityTask<UGAT_SetWeapon>(OwningAbility, TaskInstanceName);
	SetWeaponTask->SiegeCharacter = SiegeCharacter;
	SetWeaponTask->weaponSlot = weaponSlot;
	return SetWeaponTask;
}

void UGAT_SetWeapon::Activate()
{
	if (!Ability)
	{
		OnFailed.Broadcast();
		return;
	}
	
	if (SiegeCharacter && SiegeCharacter->GetWeaponInventoryComponent())
	{
		SiegeCharacter->GetWeaponInventoryComponent()->SetCurrentWeapon(weaponSlot);
		OnCompleted.Broadcast();
	}
	else
	{
		OnFailed.Broadcast();
		return;
	}
}
