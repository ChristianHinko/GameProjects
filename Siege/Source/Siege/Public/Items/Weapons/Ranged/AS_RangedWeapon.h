// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/AttributeSets/AS_BaseWeapon.h"

#include "AS_RangedWeapon.generated.h"



/**
 * 
 */
UCLASS()
class SIEGE_API UAS_RangedWeapon : public UAS_BaseWeapon
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Attributes", ReplicatedUsing = OnRep_FireRate)
		FGameplayAttributeData FireRate;
	ATTRIBUTE_ACCESSORS(UAS_RangedWeapon, FireRate)

protected:
	UFUNCTION()
		virtual void OnRep_FireRate(const FGameplayAttributeData& ServerBaseValue);

};
