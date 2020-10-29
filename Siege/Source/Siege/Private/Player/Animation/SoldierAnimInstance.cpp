// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierAnimInstance.h"

#include "UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SiegeCharacter.h"
#include "WeaponInventory.h"
#include "BaseWeapon.h"
#include "Camera/CameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"



void USoldierAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = TryGetPawnOwner();
}

void USoldierAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (IsValid(OwningPawn))
	{
		ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(OwningPawn);


		pawnSpeed = OwningPawn->GetVelocity().Size();

		bIsInAir = OwningPawn->GetMovementComponent()->IsFalling();

		pawnSpeedZ = UKismetMathLibrary::Abs(OwningPawn->GetVelocity().Z);
		
		if (OwningPawn->GetLocalRole() == ROLE_AutonomousProxy)
		{
			CameraRot = SiegeCharacter->GetPOVCamera()->GetComponentRotation();
		}
		else if (OwningPawn->GetLocalRole() == ROLE_SimulatedProxy)
		{
			CameraRot = SiegeCharacter->GetControllerRotation();
			SiegeCharacter->GetControlRotation();
		}

		if (SiegeCharacter->GetWeaponInventoryComponent() && SiegeCharacter->GetWeaponInventoryComponent()->GetCurrentWeapon())
		{
			WeaponType = SiegeCharacter->GetWeaponInventoryComponent()->GetCurrentWeapon()->WeaponInfo.WeaponType;
		}
	}
}
