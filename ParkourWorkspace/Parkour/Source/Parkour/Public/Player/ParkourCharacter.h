// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ParkourCharacter.generated.h"


class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;



/**
 * Base class for all characters in project
 */
UCLASS(config=Game)
class AParkourCharacter : public ACharacter
{
	GENERATED_BODY()


	AParkourCharacter();
public:

	UPROPERTY(VisibleDefaultsOnly, Category = Character)
	USkeletalMeshComponent* POVMesh; // should this exist on every character (including AI)?
	
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	USpringArmComponent* CameraBoom;


	USkeletalMeshComponent* GetPOVMesh() const { return POVMesh; }
	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }
protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma region Input Events
	//Actions
	void OnPressedJump();
	void OnReleasedJump();
	
	//Axis
	void MoveForward(float Value);
	void MoveRight(float Value);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
#pragma endregion
	

private:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	float BaseLookUpRate;

};

