// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiegeAbilityTask.h"

#include "GAT_Fire.generated.h"


class ASiegeCharacter;



DECLARE_MULTICAST_DELEGATE(FUGAT_FireDelegate);


/**
 * Asynchronous weapon fire ability handling
 */
UCLASS()
class SIEGE_API UGAT_Fire : public USiegeAbilityTask
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "AbilityTask", meta = (DisplayName = "ExecuteFireTask", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UGAT_Fire* CreateFireTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, ASiegeCharacter* SiegeCharacter);

	FUGAT_FireDelegate OnCompleted;
	FUGAT_FireDelegate OnFailed;
private:
	virtual void Activate() override;

	ASiegeCharacter* SiegeCharacter;
};
