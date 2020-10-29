// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiegeAbilityTask.h"

#include "GAT_SwitchWeapon.generated.h"


class ASiegeCharacter;



DECLARE_MULTICAST_DELEGATE(FUGAT_SwitchWeaponDelegate);


/**
 * Asynchronous weapon switch ability handling
 */
UCLASS()
class SIEGE_API UGAT_SwitchWeapon : public USiegeAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "AbilityTask", meta = (DisplayName = "ExecuteSwitchWeaponTask", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UGAT_SwitchWeapon* CreateSwitchWeaponTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, ASiegeCharacter* SiegeCharacter);

	FUGAT_SwitchWeaponDelegate OnCompleted;
	FUGAT_SwitchWeaponDelegate OnFailed;
private:
	virtual void Activate() override;

	ASiegeCharacter* SiegeCharacter;
};
