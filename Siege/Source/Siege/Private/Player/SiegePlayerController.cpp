// Fill out your copyright notice in the Description page of Project Settings.


#include "SiegePlayerController.h"

#include "SiegeCharacter.h"
#include "SiegePlayerState.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"



void ASiegePlayerController::BeginPlay()
{
	Super::BeginPlay();


	ASiegePlayerState* SiegePlayerState = Cast<ASiegePlayerState>(PlayerState);

	if (SiegePlayerState)
	{
		SiegePlayerState->OnCurrentCharacterInfoChange.AddDynamic(this, &ASiegePlayerController::UpdateCharacterFromInfo);
	}
}

// called from the SERVER
void ASiegePlayerController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);


	UpdateCharacterFromInfo();
}

// called from the CLIENT
void ASiegePlayerController::AcknowledgePossession(APawn* PossessedPawn)
{
	Super::AcknowledgePossession(PossessedPawn);


	UpdateCharacterFromInfo();

	ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(PossessedPawn);
	if (SiegeCharacter)
	{
		SiegeCharacter->OnPossessedByClient(this);
	}
}

// If trying to change character make sure to unpossess and destroy current character
void ASiegePlayerController::UpdateCharacterFromInfo()
{
	// We have the new CharacterInfo available if there is one (It would be stored in SiegePlayerState::CurrentCharacterInfo)
	ASiegePlayerState* SiegePlayerState = Cast<ASiegePlayerState>(PlayerState);
	ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(GetPawn());
	if (!SiegeCharacter && HasAuthority())
	{
		
		if (GetPawn()) // We're possessing something else other than ASiegeCharacter for some reason
		{
			return; // I'll figure out what to do in this case later
		}

		FActorSpawnParameters CharacterSpawnParameters;
		CharacterSpawnParameters.Owner = this;
		CharacterSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if (SiegePlayerState->GetCurrentCharacterInfo()->CharacterClass)
		{
			SiegeCharacter = GetWorld()->SpawnActor<ASiegeCharacter>(SiegePlayerState->GetCurrentCharacterInfo()->CharacterClass, CharacterSpawnParameters);   //Spawn new character as Character
			if (SiegeCharacter)
			{
				Possess(SiegeCharacter);
			}

		}
	}
	if (SiegePlayerState && SiegeCharacter)
	{
		if (SiegePlayerState->GetCurrentCharacterInfo() == nullptr && SiegePlayerState->GetCharacterInfos().IsValidIndex(0))
		{
			SiegePlayerState->SetCurrentCharacterInfo(SiegePlayerState->GetCharacterInfos()[0]);    // If CurrentCharacterInfo is null, assign it CharacterInfos[0]
		}


		if (SiegePlayerState->TryInjectDataIntoCharacter(SiegeCharacter))
		{
			GLog->Log(ELogVerbosity::Display, FString::Printf(TEXT("%s: Data injected into new character"), *GetNameSafe(this)));
		}
	}
}
