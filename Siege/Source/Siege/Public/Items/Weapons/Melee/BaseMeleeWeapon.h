// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/BaseWeapon.h"

#include "BaseMeleeWeapon.generated.h"



USTRUCT()
struct FMeleeWeaponStats
{
	GENERATED_BODY()

public:

	/**
	 * Melee weapon damage (infinite by default)
	 */
	UPROPERTY(EditDefaultsOnly, meta = (editcondition = "bOverride_damage"))
		float damage;
	UPROPERTY(EditDefaultsOnly, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		bool bOverride_damage;

	/**
	 * Melee weapon attack speed in milliseconds (instant by default)
	 */
	UPROPERTY(EditDefaultsOnly, meta = (editcondition = "bOverride_attackSpeed"))
		float attackSpeed;
	UPROPERTY(EditDefaultsOnly, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
		bool bOverride_attackSpeed;

};


/**
 * Base class for all melee weapons
 */
UCLASS()
class SIEGE_API ABaseMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	ABaseMeleeWeapon();

	UPROPERTY(EditDefaultsOnly)
	FMeleeWeaponStats MeleeWeaponStats;
};
