// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTask/SiegeAbilityTask.h"

#include "GAT_SetWeapon.generated.h"


class ASiegeCharacter;



DECLARE_MULTICAST_DELEGATE(FUGAT_SwitchWeaponDelegate)


/**
 * Asynchronous current weapon switching handling for SetWeapon ability
 */
UCLASS()
class SIEGE_API UGAT_SetWeapon : public USiegeAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "AbilityTask", meta = (DisplayName = "ExecuteSetWeaponTask", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UGAT_SetWeapon* CreateSetWeaponTask(UGameplayAbility* OwningAbility, FName TaskInsanceName, ASiegeCharacter* SiegeCharacter, uint8 weaponSlot);

	FUGAT_SwitchWeaponDelegate OnCompleted;
	FUGAT_SwitchWeaponDelegate OnFailed;
private:
	virtual void Activate() override;

	ASiegeCharacter* SiegeCharacter;
	uint8 weaponSlot;
};
