// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "BarricadeAnimInstance.generated.h"



/**
 * 
 */
UCLASS()
class SIEGE_API UBarricadeAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimationVariables)
		bool bIsBuilt;

	
protected:
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

};
