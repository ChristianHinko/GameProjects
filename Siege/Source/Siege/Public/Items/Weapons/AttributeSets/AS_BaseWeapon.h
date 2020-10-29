// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeSet/SiegeAttributeSet.h"
#include "AbilitySystemComponent.h"

#include "AS_BaseWeapon.generated.h"



/**
 * 
 */
UCLASS()
class SIEGE_API UAS_BaseWeapon : public USiegeAttributeSet
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Attributes", ReplicatedUsing = OnRep_Damage)
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAS_BaseWeapon, Damage)


protected:
	UFUNCTION()
		virtual void OnRep_Damage(const FGameplayAttributeData& ServerBaseValue);
};
