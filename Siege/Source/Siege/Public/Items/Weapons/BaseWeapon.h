// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilitySpec.h"

#include "BaseWeapon.generated.h"


class USiegeAbilitySystemComponent;
class USiegeGameplayAbility;
class ASiegeCharacter;
class UAS_BaseWeapon;
class UStaticMeshComponent;
class USkeletalMeshComponent;



/**
 * Enum used to determine which type of weapon animation to play
 */
UENUM()
enum EWeaponType
{
	WEAPONTYPE_None,
	WEAPONTYPE_Pistol,
	WEAPONTYPE_Shotgun,
	WEAPONTYPE_MeleeBig,
	WEAPONTYPE_MeleeSmall
};

USTRUCT()
struct FWeaponInfo
{
	GENERATED_BODY()

public:
	FString WeaponName;

	EWeaponType WeaponType;
};

UCLASS()
class SIEGE_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseWeapon();


	FWeaponInfo WeaponInfo;

	virtual void Fire();

	virtual void OnEquip();

	virtual void OnDequip();

	virtual void OnAddedToInventory(ASiegeCharacter* OwningCharacter);
	
	virtual void OnRemovedFromInventory();

protected:
	ASiegeCharacter* OwningSiegeCharacter;
	USiegeAbilitySystemComponent* OwningSiegeAbilitySystemComponent;

	FGameplayAbilitySpecHandle FireAbilitySpecHandle;

	UAS_BaseWeapon* WeaponAttributeSet;

	UPROPERTY(VisibleAnywhere, Category = StaticMesh)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, Category = SkeletalMesh)
		USkeletalMeshComponent* SkeletalMesh;


	// Events //////////

	virtual void PostInitializeComponents() override;

	////////////////////
};
