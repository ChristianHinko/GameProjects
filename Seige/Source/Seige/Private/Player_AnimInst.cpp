// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInst.h"





void UPlayer_AnimInst::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = TryGetPawnOwner();
}

void UPlayer_AnimInst::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if(IsValid(OwningPawn))
	{
		pawnSpeed = OwningPawn->GetVelocity().Size();
	}
}