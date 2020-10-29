// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_BaseWeapon.h"

#include "UnrealNetwork.h"



void UAS_BaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAS_BaseWeapon, Damage);
}

void UAS_BaseWeapon::OnRep_Damage(const FGameplayAttributeData& ServerBaseValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_BaseWeapon, Damage, ServerBaseValue);
}
