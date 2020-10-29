// Fill out your copyright notice in the Description page of Project Settings.


#include "SeigeCharacterMovementComponent.h"
#include "SeigeCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

USeigeCharacterMovementComponent::USeigeCharacterMovementComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{

}

void USeigeCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector & OldLocation, const FVector & OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	if (bRequestMaxWalkSpeedChange)
	{
		bRequestMaxWalkSpeedChange = false;
		MaxWalkSpeed = MyNewWalkSpeed;
	}

	//switch (CustomMovementSubMode)
	//{
	//case CMSM_None:
	//	UKismetSystemLibrary::PrintString(this, "None");
	//	break;
	//case CMSM_WallClimb:
	//	UKismetSystemLibrary::PrintString(this, "WallClimb");
	//	break;
	//default:
	//	break;
	//}
}

void USeigeCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)//Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestMaxWalkSpeedChange = (Flags& FSavedMove_Character::FLAG_Custom_0) != 0;
}

class FNetworkPredictionData_Client* USeigeCharacterMovementComponent::GetPredictionData_Client() const
{
	UKismetSystemLibrary::PrintString(GetCharacterOwner());
	check(GetPawnOwner() != NULL);
	check(GetPawnOwner()->GetLocalRole() < ROLE_Authority);

	if (!ClientPredictionData)
	{
		USeigeCharacterMovementComponent* MutableThis = const_cast<USeigeCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_SeigeCharacter(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void USeigeCharacterMovementComponent::FSavedMove_SeigeCharacter::Clear()
{
	Super::Clear();

	bSavedRequestMaxWalkSpeedChange = false;
}

uint8 USeigeCharacterMovementComponent::FSavedMove_SeigeCharacter::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange)
	{
		Result = Result | FLAG_Custom_0;
	}

	return Result;
}

bool USeigeCharacterMovementComponent::FSavedMove_SeigeCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This depends on the bit flags that are used
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_SeigeCharacter*)& NewMove)->bSavedRequestMaxWalkSpeedChange)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void USeigeCharacterMovementComponent::FSavedMove_SeigeCharacter::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	USeigeCharacterMovementComponent* CharacterMovement = Cast<USeigeCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMaxWalkSpeedChange = CharacterMovement->bRequestMaxWalkSpeedChange;
	}
}

void USeigeCharacterMovementComponent::FSavedMove_SeigeCharacter::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	USeigeCharacterMovementComponent* CharacterMovement = Cast<USeigeCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		//Prep move

	}
}

USeigeCharacterMovementComponent::FNetworkPredictionData_Client_SeigeCharacter::FNetworkPredictionData_Client_SeigeCharacter(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr USeigeCharacterMovementComponent::FNetworkPredictionData_Client_SeigeCharacter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_SeigeCharacter());
}

float USeigeCharacterMovementComponent::GetMaxWalkSpeed()
{
	if (bRequestMaxWalkSpeedChange == true)
	{
		return MyNewWalkSpeed;
	}
	else
	{
		return MaxWalkSpeed;
	}
}

//Set max walkspeed RPC to transfer	current max walkspeed from OwningClient to Server
bool USeigeCharacterMovementComponent::ServerSetMaxWalkSpeed_Validate(const float NewMaxWalkspeed)
{
	// VALIDATE STUFF
	return true;
}

void USeigeCharacterMovementComponent::ServerSetMaxWalkSpeed_Implementation(const float NewMaxWalkSpeed)
{
	MyNewWalkSpeed = NewMaxWalkSpeed;
}

void USeigeCharacterMovementComponent::SetMaxWalkSpeed(float NewMaxWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		MyNewWalkSpeed = NewMaxWalkSpeed;
		ServerSetMaxWalkSpeed(NewMaxWalkSpeed);
	}

	bRequestMaxWalkSpeedChange = true;
}



//
//void USeigeCharacterMovementComponent::OnMovementModeChanged(EMovementMode prevMovement, uint8 prevCustomMovement)
//{
//	// parent
//	Super::OnMovementModeChanged(prevMovement, prevCustomMovement);
//
//	// custom
//	switch (CustomMovementSubMode)
//	{
//	case CMSM_None:
//		
//		break;
//	case CMSM_WallClimb:
//		
//		break;
//	default:
//		break;
//	}
//}
//
//void USeigeCharacterMovementComponent::SetCustomMovementSubMode(ECustomMovementSubMode newCustomMovementSubMode)
//{
//	CustomMovementSubMode = newCustomMovementSubMode;
//}
//
//void USeigeCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
//{
//	switch(CustomMovementSubMode)
//	{
//	case CMSM_None:
//		PhysCustomNone(deltaTime, Iterations);
//		break;
//	case CMSM_WallClimb:
//		PhysCustomWallClimb(deltaTime, Iterations);
//		break;
//	default:
//		break;
//	}
//}
//
//
//void USeigeCharacterMovementComponent::PhysCustomNone(float deltaTime, int32 Iterations)
//{
//	// parent
//	Super::PhysCustom(deltaTime, Iterations);
//
//	// custom
//	//UKismetSystemLibrary::PrintString(this, "None");
//}
//void USeigeCharacterMovementComponent::PhysCustomWallClimb(float deltaTime, int32 Iterations)
//{
//	// parent
//	Super::PhysCustom(deltaTime, Iterations);
//
//	// custom
//	//UKismetSystemLibrary::PrintString(this, "WallClimb");
//}
