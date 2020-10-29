// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "SiegeAbilitySystemComponent.h"

#include "SiegePlayerState.generated.h"


class UCharacterAttributeSet;
class ASiegeCharacter;
class ASiegePlayerController;



/**
 * The controller can use this to know when the current character is changed, and recreate or modify its pawn as needed (by calling UpdateCharacter())
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterInfoChange);

/**
 * This should store the info for a character. A player may switch characters by
 * calling SetCurrentCharacterInfo(FCharacterInfo NewCharacterInfo), and then calling UpdateCharacter().
 */
USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = CharacterInfo)
		TSubclassOf<ASiegeCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = CharacterInfo)
		FString Name;
};

/**
 * Stores the available characters and the current character for the player
 */
UCLASS()
class SIEGE_API ASiegePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ASiegePlayerState();

	/**
	 * When the player starts the game, this might contact an online service to get
	 * the list of characters, so that they can select one in the menu.
	 * Currently, not much use since you can popultat it in the playerstate blueprint
	 */
	void PopulateCharacterInfo();

	/**
	 * Set a new character to be active. Can be called on client
	 */
    void SetCurrentCharacterInfo(FCharacterInfo NewCharacterInfo);

    UFUNCTION(Server, Reliable, WithValidation)
        void Server_SetCharacterInfo(FCharacterInfo NewCharacterInfo);

    UPROPERTY()
		FCharacterInfoChange OnCurrentCharacterInfoChange;

	/**
	 * Sets up the ASC on the new character so you can do Ability system login within the character
	 * Also applies the current character info to the new character. Should be called on possession on both client
	 * and server.
	 */
	bool TryInjectDataIntoCharacter(class ASiegeCharacter* Character);


	// Will be null if no character has been selected yet
    FCharacterInfo* GetCurrentCharacterInfo();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override { return Cast<UAbilitySystemComponent>(SiegeAbilitySystemComponent); }

    UCharacterAttributeSet* GetCharacterAttributeSet() const { return CharacterAttributeSet; }

	TArray<FCharacterInfo> GetCharacterInfos() const { return CharacterInfos; }

	float GetCurrentLevel() const { return level; }

protected:
	ASiegePlayerController* SiegePlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentCharacterInfo)
		FCharacterInfo CurrentCharacterInfo;

	UFUNCTION()
		void OnRep_CurrentCharacterInfo();


	USiegeAbilitySystemComponent* SiegeAbilitySystemComponent;

	UPROPERTY()
		UCharacterAttributeSet* CharacterAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<FCharacterInfo> CharacterInfos;

	UPROPERTY(ReplicatedUsing = OnRep_Level)
		float level;

	UFUNCTION()
		void OnRep_Level();

private:
	bool bInitialized;

};
