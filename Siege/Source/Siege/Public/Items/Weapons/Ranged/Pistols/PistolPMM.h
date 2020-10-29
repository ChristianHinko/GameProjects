// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/Ranged/BaseRangedWeapon.h"

#include "PistolPMM.generated.h"



/**
 * 
 */
UCLASS()
class SIEGE_API APistolPMM : public ABaseRangedWeapon
{
	GENERATED_BODY()
	
public:
	APistolPMM();

protected:
	UPROPERTY(EditAnywhere, Category = Animation)
		UAnimSequence* FireAnim;

	virtual void Fire() override;

	virtual void OnEquip() override;

	virtual void OnDequip() override;

	virtual void OnAddedToInventory(ASiegeCharacter* OwningCharacter) override;

};
