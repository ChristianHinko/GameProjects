// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SiegeAttributeSet.h"

#include "CharacterAttributeSet.generated.h"



/**
 * 
 */
UCLASS()
class SIEGE_API UCharacterAttributeSet : public USiegeAttributeSet
{
	GENERATED_BODY()

public:

	// Constructor (default values)
	UCharacterAttributeSet();

	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
	UPROPERTY(VisibleAnywhere, Category = "Attributes", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Health)

	/** Max Health, caps current health*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHealth)

		/** Max Health, caps current health*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes", ReplicatedUsing = OnRep_MoveSpeed)
		FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MoveSpeed)

protected:
	// Events //////////
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	////////////////////

	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& ServerBaseValue);

	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& ServerBaseValue);

	UFUNCTION()
		virtual void OnRep_MoveSpeed(const FGameplayAttributeData& ServerBaseValue);
};
