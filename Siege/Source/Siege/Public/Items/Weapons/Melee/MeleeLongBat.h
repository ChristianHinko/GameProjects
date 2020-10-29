// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Melee/BaseMeleeWeapon.h"

#include "MeleeLongBat.generated.h"



/**
 * Long ranged bat melee weapon
 */
UCLASS()
class SIEGE_API AMeleeLongBat : public ABaseMeleeWeapon
{
	GENERATED_BODY()
	
public:
	AMeleeLongBat();

protected:
	virtual void Fire() override;

	virtual void OnEquip() override;

	virtual void OnDequip() override;

	virtual void OnAddedToInventory(ASiegeCharacter* OwningCharacter) override;

};
