// Fill out your copyright notice in the Description page of Project Settings.


#include "AS_Barricade.h"

#include "UnrealNetwork.h"



void UAS_Barricade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UAS_Barricade, Health);
	DOREPLIFETIME(UAS_Barricade, MaxHealth);
}



UAS_Barricade::UAS_Barricade()
{

}

void UAS_Barricade::OnRep_Health(const FGameplayAttributeData& ServerBaseValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Barricade, Health, ServerBaseValue);
}

void UAS_Barricade::OnRep_MaxHealth(const FGameplayAttributeData& ServerBaseValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Barricade, MaxHealth, ServerBaseValue);
}
