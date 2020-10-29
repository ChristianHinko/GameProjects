// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "SiegeAbilitySystemComponent.h"
#include "CharacterAttributeSet.h"

#include "SiegeCharacter.generated.h"


class UGameplayAbility;
class UGA_Fire;
class USiegeCharacterMovementComponent;
class UWeaponInventory;
class UCameraComponent;



UCLASS()
class ASiegeCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Self-aware mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent* SelfMesh;

	/** First person camera */
	UPROPERTY(VisibleDefaultsOnly)
		UCameraComponent* POVCamera;

	UPROPERTY(VisibleDefaultsOnly)
		UWeaponInventory* WeaponInventoryComponent;

public:
	/**
	 * Constructor with post construction initialization properties
	 */
	ASiegeCharacter(const FObjectInitializer& PCIP);

	// public protected GETTERS //////////
	USiegeAbilitySystemComponent* GetAbilitySystemComponent() const override { return SiegeAbilitySystemComponent; };

	void SetAbilitySystemComponent(USiegeAbilitySystemComponent* NewSiegeAbilitySystemComponent) { SiegeAbilitySystemComponent = NewSiegeAbilitySystemComponent; }

	UCharacterAttributeSet* GetCharacterAttributeSet() const { return CharacterAttributeSet; }

	USiegeCharacterMovementComponent* GetSiegeCharacterMovementComponent() const { return SiegeCharacterMovementComponent; }

	UWeaponInventory* GetWeaponInventoryComponent() const { return WeaponInventoryComponent; }

	USkeletalMeshComponent* GetSelfMesh() const { return SelfMesh; }

	UCameraComponent* GetPOVCamera() const { return POVCamera; }

	FRotator GetControllerRotation() const { return ControllerRotation; }
	//////////////////////////////

	virtual void OnPossessedByClient(AController* NewController);

    virtual void OnAbilityDataInjected();

	// Attributes //////////
	virtual float GetHealth() const { return GetCharacterAttributeSet()->GetHealth(); };
	virtual void OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	
	virtual float GetMaxHealth() const { return GetCharacterAttributeSet()->GetMaxHealth(); };
	virtual void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	////////////////////////


	// Abilities
	UPROPERTY(EditDefaultsOnly, Category = AbilitySystem)
		TSubclassOf<UGameplayAbility> FireAbility;
	UPROPERTY(Replicated)
		FGameplayAbilitySpecHandle FireAbilitySpecHandle;

	UPROPERTY(EditDefaultsOnly, Category = AbilitySystem)
		TSubclassOf<UGameplayAbility> SwitchWeaponAbility;
	UPROPERTY(Replicated)
		FGameplayAbilitySpecHandle SwitchWeaponAbilitySpecHandle;

	UPROPERTY(EditDefaultsOnly, Category = AbilitySystem)
		TSubclassOf<UGameplayAbility> SetWeaponAbility;
	UPROPERTY(Replicated)
		FGameplayAbilitySpecHandle SetWeaponAbilitySpecHandle;

	UPROPERTY(EditDefaultsOnly, Category = AbilitySystem)
		TSubclassOf<UGameplayAbility> InteractAbility;
	UPROPERTY(Replicated)
		FGameplayAbilitySpecHandle InteractAbilitySpecHandle;
	/**
	 * Subtract this by 1 to take into account the 0th element in the array
	 */
	uint8 weaponSlot;

protected:
	virtual void OnRep_PlayerState() override;

	USiegeAbilitySystemComponent* SiegeAbilitySystemComponent;

	UCharacterAttributeSet* CharacterAttributeSet;

	USiegeCharacterMovementComponent* SiegeCharacterMovementComponent;

	// Events //////////////////////////////////////////////////////

	virtual void BeginPlay();

	virtual void PostInitializeComponents() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTimeX) override;

	////////////////////////////////////////////////////////////////

	UFUNCTION(Server, Reliable, WithValidation)
        void ServerSpawnDefaultInventory();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

#pragma region Inputs
	// Actions //////////
	virtual void OnJumpPressed();
	virtual void OnJumpReleased();

	virtual void OnCrouchPressed();
	virtual void OnCrouchReleased();

	virtual void OnPronePressed();
	virtual void OnProneReleased();

	virtual void OnFirePressed();
	virtual void OnFireReleased();

	virtual void OnReloadPressed();
	virtual void OnReloadReleased();

	virtual void OnADSPressed();
	virtual void OnADSReleased();

	virtual void OnSwitchWeaponPressed();
	virtual void OnSwitchWeaponReleased();

	virtual void OnTacticalMeleePressed();
	virtual void OnTacticalMeleeReleased();

	virtual void OnWeaponSlot1Pressed();
	virtual void OnWeaponSlot1Released();

	virtual void OnWeaponSlot2Pressed();
	virtual void OnWeaponSlot2Released();

	virtual void OnWeaponSlot3Pressed();
	virtual void OnWeaponSlot3Released();

	virtual void OnWeaponSlot4Pressed();
	virtual void OnWeaponSlot4Released();

	virtual void OnWeaponSlot5Pressed();
	virtual void OnWeaponSlot5Released();

	virtual void OnWeaponSlot6Pressed();
	virtual void OnWeaponSlot6Released();

	virtual void OnWeaponSlot7Pressed();
	virtual void OnWeaponSlot7Released();

	virtual void OnWeaponSlot8Pressed();
	virtual void OnWeaponSlot8Released();

	virtual void OnWeaponSlot9Pressed();
	virtual void OnWeaponSlot9Released();

	virtual void OnInteractPressed();
	virtual void OnInteractReleased();
	/////////////////////
#pragma endregion

	UPROPERTY(Replicated)
		FRotator ControllerRotation;

private:


	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);
	float BaseTurnRate;

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
	float BaseLookUpRate;

};
