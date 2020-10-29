// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_RangedWeapon.h"

#include "UnrealNetwork.h"



void UAS_RangedWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAS_RangedWeapon, FireRate);
}

void UAS_RangedWeapon::OnRep_FireRate(const FGameplayAttributeData& ServerBaseValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_RangedWeapon, FireRate, ServerBaseValue);
}
