// Fill out your copyright notice in the Description page of Project Settings.


#include "SiegePlayerState.h"

#include "UnrealNetwork.h"
#include "CharacterAttributeSet.h"
#include "SiegeAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SiegePlayerController.h"
#include "SiegeCharacter.h"



void ASiegePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASiegePlayerState, CurrentCharacterInfo);
	DOREPLIFETIME(ASiegePlayerState, level);
}

ASiegePlayerState::ASiegePlayerState()
{
	SiegeAbilitySystemComponent = CreateDefaultSubobject<USiegeAbilitySystemComponent>(TEXT("SiegeAbilitySystemComponent"));
	SiegeAbilitySystemComponent->SetIsReplicated(true);
	SiegeAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));
}

void ASiegePlayerState::PopulateCharacterInfo()
{
	// We would use this if we decide sometime to add the infos through code, not BP (ie: get them from a server)



	bInitialized = true;
}

void ASiegePlayerState::SetCurrentCharacterInfo(FCharacterInfo NewCharacterInfo)
{
	if (!bInitialized)
	{
		PopulateCharacterInfo();
	}

	if (HasAuthority())
	{
		CurrentCharacterInfo = NewCharacterInfo;
		OnRep_CurrentCharacterInfo();
	}
	else
	{
		Server_SetCharacterInfo(NewCharacterInfo);
	}
}

void ASiegePlayerState::OnRep_CurrentCharacterInfo()
{
	OnCurrentCharacterInfoChange.Broadcast();
}

bool ASiegePlayerState::Server_SetCharacterInfo_Validate(FCharacterInfo NewCharacterInfo)
{
	return true;
}
void ASiegePlayerState::Server_SetCharacterInfo_Implementation(FCharacterInfo NewCharacterInfo)
{
	SetCurrentCharacterInfo(NewCharacterInfo);
}

bool ASiegePlayerState::TryInjectDataIntoCharacter(ASiegeCharacter* Character)
{
	if (GetCurrentCharacterInfo() == nullptr || !Character)
	{
		return false;
	}

	Character->SetAbilitySystemComponent(SiegeAbilitySystemComponent);
	if (SiegeAbilitySystemComponent)
	{
		SiegeAbilitySystemComponent->InitAbilityActorInfo(this, Character);
		if (HasAuthority())
		{
			SiegeAbilitySystemComponent->ClearAllAbilities();
		}
	}

	Character->OnAbilityDataInjected();
	return true;
}

void ASiegePlayerState::OnRep_Level()
{

}


FCharacterInfo* ASiegePlayerState::GetCurrentCharacterInfo()
{
	if (&CurrentCharacterInfo)
	{
		return &CurrentCharacterInfo;
	}
	else
	{
		return nullptr;
	}
}
