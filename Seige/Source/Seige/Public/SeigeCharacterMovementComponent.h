// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SeigeCharacterMovementComponent.generated.h"

/**
 * 
 */

//enum ECustomMovementSubMode : uint8
//{
//	CMSM_None,
//	CMSM_WallClimb
//};

UCLASS()
class SEIGE_API USeigeCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

	class FSavedMove_SeigeCharacter : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// Walk speed update
		uint8 bSavedRequestMaxWalkSpeedChange : 1;
	};

	class FNetworkPredictionData_Client_SeigeCharacter : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_SeigeCharacter(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	void OnMovementUpdated(float DeltaSeconds, const FVector & OldLocation, const FVector & OldVelocity);

	// Set max walkspeed
	uint8 bRequestMaxWalkSpeedChange : 1;

	UFUNCTION(Unreliable, Server, WithValidation)
		void ServerSetMaxWalkSpeed(const float NewMaxWalkSpeed);

	float MyNewWalkSpeed;

	float GetMaxWalkSpeed();
	void SetMaxWalkSpeed(float NewMaxWalkSpeed);

//public:
	//ECustomMovementSubMode CustomMovementSubMode;
	//void SetCustomMovementSubMode(ECustomMovementSubMode newCustomMovementSubMode);

//protected:
	//void PhysCustom(float deltaTime, int32 Iterations) override;


	//UFUNCTION()
		//void OnMovementModeChanged(EMovementMode prevMovement, uint8 prevCustomMovement);


	//void PhysCustomNone(float deltaTime, int32 Iterations);
	//void PhysCustomWallClimb(float deltaTime, int32 Iterations);
};
