// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SiegeAttributeSet.h"
#include "SiegeAbilitySystemComponent.h"

#include "AS_Barricade.generated.h"



/**
 * 
 */
UCLASS()
class SIEGE_API UAS_Barricade : public USiegeAttributeSet
{
	GENERATED_BODY()
	
public:
	// construct (defaut values)
	UAS_Barricade();

	/** Current Health, when 0 we expect barricade to die. Capped by MaxHealth */
	UPROPERTY(VisibleAnywhere, Category = "Attributes", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAS_Barricade, Health)

		/** Max Health, caps current health*/
	UPROPERTY(VisibleAnywhere, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAS_Barricade, MaxHealth)

protected:

	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& ServerBaseValue);

	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& ServerBaseValue);
};
