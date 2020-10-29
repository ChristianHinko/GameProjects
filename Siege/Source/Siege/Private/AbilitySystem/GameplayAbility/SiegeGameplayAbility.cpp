// Fill out your copyright notice in the Description page of Project Settings.


#include "SiegeGameplayAbility.h"



USiegeGameplayAbility::USiegeGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
}
