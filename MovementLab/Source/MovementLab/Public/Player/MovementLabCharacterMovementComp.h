// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "MovementLabCharacterMovementComp.generated.h"



/**
 * Custom movement modes. used when MovementMode == MOVE_Custom
 */
UENUM()
enum ECustomMovementMode
{
	CMOVE_None				UMETA(DisplayName="NoneCustom"),
	CMOVE_InfiniteWalk		UMETA(DisplayName="InfiniteWalk"),
	CMOVE_MAX				UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class MOVEMENTLAB_API UMovementLabCharacterMovementComp : public UCharacterMovementComponent
{
	GENERATED_BODY()


	friend class FSavedMove_MovementLabCharacter;

public:
	UMovementLabCharacterMovementComp();


	UFUNCTION()
		void RequestSetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0);

	UFUNCTION()
		void RequestSetMaxWalkSpeed(float NewMaxWalkSpeed);

protected:

	//BEGIN UCharacterMovementComponent Interface
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float deltaTime, const FVector& OldLocation, const FVector& OldVelocity) override;
	virtual bool IsMovingOnGround() const override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMaxBrakingDeceleration() const override;
	virtual FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const override;
	virtual FString GetMovementName() const override;
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	//END UCharacterMovementComponent Interface

	//BEGIN UMovementComponent Interface
	virtual float GetMaxSpeed() const override;
	//END UMovementComponent Interface


	TEnumAsByte<ECustomMovementMode> CustomMovementMode;


	// movement modes
	virtual void PhysInfiniteWalk(float deltaTime, int32 Iterations);
	

	// prediction requests:

	uint8 bRequestMovementModeChange : 1;
	UFUNCTION(Unreliable, Server, WithValidation)
		void Server_RequestSetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0);
	EMovementMode PendingMovementMode;
	ECustomMovementMode PendingCustomMovementMode;

	uint8 bRequestMaxWalkSpeedChange : 1;
	UFUNCTION(Unreliable, Server, WithValidation)
		void Server_RequestSetMaxWalkSpeed(const float NewMaxWalkSpeed);
	float PendingNewMaxWalkSpeed;
};


class FSavedMove_MovementLabCharacter : public FSavedMove_Character
{
public:
	typedef FSavedMove_Character Super;


	virtual void Clear() override;
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	virtual void PrepMoveFor(class ACharacter* Character) override;
	virtual uint8 GetCompressedFlags() const override;

protected:
	uint8 bSavedRequestMovementModeChange : 1;

	uint8 bSavedRequestMaxWalkSpeedChange : 1;
};

class FNetworkPredictionData_Client_MovementLabCharacter : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;


	FNetworkPredictionData_Client_MovementLabCharacter(const UCharacterMovementComponent& ClientMovement);

	virtual FSavedMovePtr AllocateNewMove() override;
};
