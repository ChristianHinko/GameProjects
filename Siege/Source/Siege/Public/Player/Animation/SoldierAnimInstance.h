// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseWeapon.h"

#include "SoldierAnimInstance.generated.h"



/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class SIEGE_API USoldierAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		float pawnSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		bool bIsInAir;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		float pawnSpeedZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		uint8 WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		FRotator CameraRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		float ShoulderXRot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		float HandXRot;

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

private:
	APawn* OwningPawn;

};
