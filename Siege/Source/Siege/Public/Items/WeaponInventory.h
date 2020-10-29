// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "WeaponInventory.generated.h"


class ASiegeCharacter;
class ABaseWeapon;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIEGE_API UWeaponInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponInventory();
	

	
	void SpawnDefaultInventory();

	void SetCurrentWeapon(ABaseWeapon* NewCurrentWeapon);
	void SetCurrentWeapon(int32 NewCurrentWeaponIndex);

	void SwitchWeapon();

	void AddWeaponToInventory(ABaseWeapon* WeaponToAdd, bool equipWeapon = true);

	void RemoveWeaponFromInventory(ABaseWeapon* WeaponToRemove, bool removeSpecificInstance);

	// public protected GETTERS //////////
	TArray<ABaseWeapon*> GetWeaponInventory() const { return WeaponInventory; }

	ABaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	ABaseWeapon* GetPreviousWeapon() const { return PreviousWeapon; }
	//////////////////////////////////////

protected:
	virtual void InitializeComponent() override;


	ASiegeCharacter* SiegeCharacter;

	UPROPERTY(ReplicatedUsing=OnRepWeaponInventory)
		TArray<ABaseWeapon*> WeaponInventory;
	UFUNCTION()
		void OnRepWeaponInventory(TArray<ABaseWeapon*> PreRepInventory);

	UPROPERTY(Replicated)
		ABaseWeapon* CurrentWeapon;

	UPROPERTY(Replicated)
		ABaseWeapon* PreviousWeapon;

	/**
	 * Third last weapon used (if PreviousWeapon is NULL, it will be set to ThirdLastWeapon)
	 */
	ABaseWeapon* ThirdLastWeapon;

	UPROPERTY(EditDefaultsOnly, Category = Weapons)
		TArray<TSubclassOf<ABaseWeapon> > DefaultInventoryClasses;

};
