// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTask/SiegeAbilityTask.h"

#include "GAT_Interact.generated.h"


class ASiegeCharacter;



DECLARE_MULTICAST_DELEGATE(FUGAT_InteractDelegate);


/**
 * 
 */
UCLASS()
class SIEGE_API UGAT_Interact : public USiegeAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "AbilityTask", meta = (DisplayName = "ExecuteInteractTask", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UGAT_Interact* CreateInteractTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, ASiegeCharacter* SiegeCharacter);

	FUGAT_InteractDelegate OnCompleted;
	FUGAT_InteractDelegate OnFailed;
private:
	virtual void Activate() override;

	ASiegeCharacter* SiegeCharacter;
};
