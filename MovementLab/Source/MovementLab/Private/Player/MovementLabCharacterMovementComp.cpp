// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementLabCharacterMovementComp.h"

#include "MovementLabCharacter.h"

#include "Kismet/KismetSystemLibrary.h"



DEFINE_LOG_CATEGORY_STATIC(LogCharacterMovementCustom, Log, All);

DECLARE_STATS_GROUP(TEXT("MovementLabCharacter"), STATGROUP_MovementLabCharacter, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("Char PhysInfiniteWalk"), STAT_CharPhysInfiniteWalk, STATGROUP_MovementLabCharacter);


UMovementLabCharacterMovementComp::UMovementLabCharacterMovementComp()
{

}

#pragma region Move Requests
void UMovementLabCharacterMovementComp::RequestSetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	if (PawnOwner->IsLocallyControlled())
	{
		PendingMovementMode = NewMovementMode;
		PendingCustomMovementMode = static_cast<TEnumAsByte<ECustomMovementMode>>(NewCustomMode);

		Server_RequestSetMovementMode(NewMovementMode, NewCustomMode);
	}

	bRequestMovementModeChange = true;
}

void UMovementLabCharacterMovementComp::RequestSetMaxWalkSpeed(float NewMaxWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		PendingNewMaxWalkSpeed = NewMaxWalkSpeed;

		Server_RequestSetMaxWalkSpeed(NewMaxWalkSpeed);
	}

	bRequestMaxWalkSpeedChange = true;
}
#pragma endregion

class FNetworkPredictionData_Client* UMovementLabCharacterMovementComp::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	check(PawnOwner->GetLocalRole() < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UMovementLabCharacterMovementComp* MutableThis = const_cast<UMovementLabCharacterMovementComp*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MovementLabCharacter(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void FSavedMove_MovementLabCharacter::Clear()
{
	Super::Clear();


	bSavedRequestMovementModeChange = false;
	bSavedRequestMaxWalkSpeedChange = false;
}

bool FSavedMove_MovementLabCharacter::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	if (bSavedRequestMovementModeChange != ((FSavedMove_MovementLabCharacter*)&NewMove)->bSavedRequestMovementModeChange)
	{
		return false;
	}
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_MovementLabCharacter*)&NewMove)->bSavedRequestMaxWalkSpeedChange)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void FSavedMove_MovementLabCharacter::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UMovementLabCharacterMovementComp* CharacterMovement = Cast<UMovementLabCharacterMovementComp>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMovementModeChange = CharacterMovement->bRequestMovementModeChange;
		bSavedRequestMaxWalkSpeedChange = CharacterMovement->bRequestMaxWalkSpeedChange;
	}
}

void FSavedMove_MovementLabCharacter::PrepMoveFor(class ACharacter* Character)	//client only (called on client RIGHT BEFORE replaying one of many saved moves in a frame during correction)
{
	Super::PrepMoveFor(Character);

	UMovementLabCharacterMovementComp* CharacterMovement = Cast<UMovementLabCharacterMovementComp>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		CharacterMovement->bRequestMovementModeChange = bSavedRequestMovementModeChange;
		CharacterMovement->bRequestMaxWalkSpeedChange = bSavedRequestMaxWalkSpeedChange;
	}
}

uint8 FSavedMove_MovementLabCharacter::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMovementModeChange)
	{
		Result |= FLAG_Custom_0;
	}
	if (bSavedRequestMaxWalkSpeedChange)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

void UMovementLabCharacterMovementComp::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	ENetRole hi = GetOwner()->GetLocalRole();

	bRequestMovementModeChange = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bRequestMaxWalkSpeedChange = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;

	if (bRequestMovementModeChange || bRequestMaxWalkSpeedChange)
	{
		int d = 0;
	}
}

void UMovementLabCharacterMovementComp::OnMovementUpdated(float deltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	//Super::OnMovementUpdated(deltaTime, OldLocation, OldVelocity);	// empty super


	// MOVEMENT FLAGS
	if (bRequestMovementModeChange)
	{
		bRequestMovementModeChange = false;
		SetMovementMode(PendingMovementMode, PendingCustomMovementMode);
	}
	if (bRequestMaxWalkSpeedChange)
	{
		bRequestMaxWalkSpeedChange = false;
		MaxWalkSpeed = PendingNewMaxWalkSpeed;
	}

	UKismetSystemLibrary::PrintString(this, GetMovementName(), true, false);
}

FNetworkPredictionData_Client_MovementLabCharacter::FNetworkPredictionData_Client_MovementLabCharacter(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_MovementLabCharacter::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_MovementLabCharacter());
}


bool UMovementLabCharacterMovementComp::IsMovingOnGround() const
{
	if (!UpdatedComponent)
	{
		return false;
	}

	if (MovementMode == MOVE_Custom)
	{
		if (CustomMovementMode == CMOVE_InfiniteWalk)
		{
			return true;
		}
	}


	return Super::IsMovingOnGround();
}

FVector UMovementLabCharacterMovementComp::NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const
{


	return Super::NewFallVelocity(InitialVelocity, Gravity, DeltaTime);
}

FString UMovementLabCharacterMovementComp::GetMovementName() const
{
	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		case CMOVE_None:				return TEXT("Custom_None"); break;
		case CMOVE_InfiniteWalk:		return TEXT("Custom_InfiniteWalk"); break;
		default:						break;
		}
	}


	return Super::GetMovementName();
}

void UMovementLabCharacterMovementComp::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	Super::SetMovementMode(NewMovementMode, NewCustomMode);


	CustomMovementMode = static_cast<TEnumAsByte<ECustomMovementMode>>(NewCustomMode);
}

void UMovementLabCharacterMovementComp::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UMovementLabCharacterMovementComp::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);


	switch (CustomMovementMode)
	{
	case CMOVE_None:
		break;
	case CMOVE_InfiniteWalk:
		PhysInfiniteWalk(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogCharacterMovementCustom, Warning, TEXT("%s has unsupported custom movement mode %d"), *CharacterOwner->GetName(), int32(CustomMovementMode));
		SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_None);
		break;
	}
}

float UMovementLabCharacterMovementComp::GetMaxSpeed() const
{
	switch (CustomMovementMode)
	{
	case CMOVE_None:
		break;
	case CMOVE_InfiniteWalk:
		return MaxWalkSpeed; break;
	default:
		break;
	}


	return Super::GetMaxSpeed();
}

float UMovementLabCharacterMovementComp::GetMaxAcceleration() const
{
	//switch (CustomMovementMode)
	//{
	//case CMOVE_None:
	//	break;
	//default:
	//	break;
	//}


	return Super::GetMaxAcceleration();
}

float UMovementLabCharacterMovementComp::GetMaxBrakingDeceleration() const
{
	switch (CustomMovementMode)
	{
	case CMOVE_None:
		break;
	case CMOVE_InfiniteWalk:
		return BrakingDecelerationWalking;
	default:
		break;
	}


	return Super::GetMaxBrakingDeceleration();
}

#pragma region Phys Custom
void UMovementLabCharacterMovementComp::PhysInfiniteWalk(float deltaTime, int32 Iterations)
{
	PhysWalking(deltaTime, Iterations);
	//SCOPE_CYCLE_COUNTER(STAT_CharPhysInfiniteWalk);

	//if (deltaTime < MIN_TICK_TIME)
	//{
	//	return;
	//}

	//if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->Role != ROLE_SimulatedProxy)))
	//{
	//	Acceleration = FVector::ZeroVector;
	//	Velocity = FVector::ZeroVector;
	//	return;
	//}

	//if (!UpdatedComponent->IsQueryCollisionEnabled())
	//{
	//	SetMovementMode(MOVE_Custom, CMOVE_InfiniteWalk);
	//	return;
	//}

	//checkCode(ensureMsgf(!Velocity.ContainsNaN(), TEXT("PhysInfiniteWalk: Velocity contains NaN before Iteration (%s)\n%s"), *GetPathNameSafe(this), *Velocity.ToString()));

	//bJustTeleported = false;
	//bool bCheckedFall = false;
	//bool bTriedLedgeMove = false;
	//float remainingTime = deltaTime;

	//// Perform the move
	//while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity() || (CharacterOwner->Role == ROLE_SimulatedProxy)))
	//{
	//	Iterations++;
	//	bJustTeleported = false;
	//	const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
	//	remainingTime -= timeTick;

	//	// Save current values
	//	UPrimitiveComponent* const OldBase = GetMovementBase();
	//	const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
	//	const FVector OldLocation = UpdatedComponent->GetComponentLocation();
	//	const FFindFloorResult OldFloor = CurrentFloor;

	//	RestorePreAdditiveRootMotionVelocity();

	//	// Ensure velocity is horizontal.
	//	MaintainHorizontalGroundVelocity();
	//	const FVector OldVelocity = Velocity;
	//	Acceleration.Z = 0.f;

	//	// Apply acceleration
	//	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	//	{
	//		CalcVelocity(timeTick, GroundFriction, false, GetMaxBrakingDeceleration());
	//		checkCode(ensureMsgf(!Velocity.ContainsNaN(), TEXT("PhysInfiniteWalk: Velocity contains NaN after CalcVelocity (%s)\n%s"), *GetPathNameSafe(this), *Velocity.ToString()));
	//	}

	//	ApplyRootMotionToVelocity(timeTick);
	//	checkCode(ensureMsgf(!Velocity.ContainsNaN(), TEXT("PhysInfiniteWalk: Velocity contains NaN after Root Motion application (%s)\n%s"), *GetPathNameSafe(this), *Velocity.ToString()));

	//	if (IsFalling())
	//	{
	//		// Root motion could have put us into Falling.
	//		// No movement has taken place this movement tick so we pass on full time/past iteration count
	//		StartNewPhysics(remainingTime + timeTick, Iterations - 1);
	//		return;
	//	}

	//	// Compute move parameters
	//	const FVector MoveVelocity = Velocity;
	//	const FVector Delta = timeTick * MoveVelocity;
	//	const bool bZeroDelta = Delta.IsNearlyZero();
	//	FStepDownResult StepDownResult;

	//	if (bZeroDelta)
	//	{
	//		remainingTime = 0.f;
	//	}
	//	else
	//	{
	//		// try to move forward
	//		MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

	//		if (IsFalling())
	//		{
	//			// pawn decided to jump up
	//			const float DesiredDist = Delta.Size();
	//			if (DesiredDist > KINDA_SMALL_NUMBER)
	//			{
	//				const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
	//				remainingTime += timeTick * (1.f - FMath::Min(1.f, ActualDist / DesiredDist));
	//			}
	//			StartNewPhysics(remainingTime, Iterations);
	//			return;
	//		}
	//		else if (IsSwimming()) //just entered water
	//		{
	//			StartSwimming(OldLocation, OldVelocity, timeTick, remainingTime, Iterations);
	//			return;
	//		}
	//	}

	//	// Update floor.
	//	// StepUp might have already done it for us.
	//	if (StepDownResult.bComputedFloor)
	//	{
	//		CurrentFloor = StepDownResult.FloorResult;
	//	}
	//	else
	//	{
	//		FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
	//	}

	//	// check for ledges here
	//	const bool bCheckLedges = !CanWalkOffLedges();
	//	if (bCheckLedges && !CurrentFloor.IsWalkableFloor())
	//	{
	//		// calculate possible alternate movement
	//		const FVector GravDir = FVector(0.f, 0.f, -1.f);
	//		const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
	//		if (!NewDelta.IsZero())
	//		{
	//			// first revert this move
	//			RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

	//			// avoid repeated ledge moves if the first one fails
	//			bTriedLedgeMove = true;

	//			// Try new movement direction
	//			Velocity = NewDelta / timeTick;
	//			remainingTime += timeTick;
	//			continue;
	//		}
	//		else
	//		{
	//			// see if it is OK to jump
	//			// @todo collision : only thing that can be problem is that oldbase has world collision on
	//			bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
	//			if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
	//			{
	//				return;
	//			}
	//			bCheckedFall = true;

	//			// revert this move
	//			RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
	//			remainingTime = 0.f;
	//			break;
	//		}
	//	}
	//	else
	//	{
	//		// Validate the floor check
	//		if (CurrentFloor.IsWalkableFloor())
	//		{
	//			if (ShouldCatchAir(OldFloor, CurrentFloor))
	//			{
	//				HandleWalkingOffLedge(OldFloor.HitResult.ImpactNormal, OldFloor.HitResult.Normal, OldLocation, timeTick);
	//				if (IsMovingOnGround())
	//				{
	//					// If still walking, then fall. If not, assume the user set a different mode they want to keep.
	//					StartFalling(Iterations, remainingTime, timeTick, Delta, OldLocation);
	//				}
	//				return;
	//			}

	//			AdjustFloorHeight();
	//			SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
	//		}
	//		else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
	//		{
	//			// The floor check failed because it started in penetration
	//			// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
	//			FHitResult Hit(CurrentFloor.HitResult);
	//			Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
	//			const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
	//			ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
	//			bForceNextFloorCheck = true;
	//		}

	//		// check if just entered water
	//		if (IsSwimming())
	//		{
	//			StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
	//			return;
	//		}

	//		// See if we need to start falling.
	//		if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
	//		{
	//			const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
	//			if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
	//			{
	//				return;
	//			}
	//			bCheckedFall = true;
	//		}
	//	}


	//	// Allow overlap events and such to change physics state and velocity
	//	if (IsMovingOnGround())
	//	{
	//		// Make velocity reflect actual move
	//		if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
	//		{
	//			// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
	//			Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick;
	//		}
	//	}

	//	// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
	//	if (UpdatedComponent->GetComponentLocation() == OldLocation)
	//	{
	//		remainingTime = 0.f;
	//		break;
	//	}
	//}

	//if (IsMovingOnGround())
	//{
	//	MaintainHorizontalGroundVelocity();
	//}
}
#pragma endregion

#pragma region Prediction Requests
bool UMovementLabCharacterMovementComp::Server_RequestSetMovementMode_Validate(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	return true;
}

void UMovementLabCharacterMovementComp::Server_RequestSetMovementMode_Implementation(EMovementMode NewMovementMode, uint8 NewCustomMode)
{
	PendingMovementMode = NewMovementMode;
	PendingCustomMovementMode = static_cast<TEnumAsByte<ECustomMovementMode>>(NewCustomMode);
}

bool UMovementLabCharacterMovementComp::Server_RequestSetMaxWalkSpeed_Validate(const float NewMaxWalkSpeed)
{
	return true;
}

void UMovementLabCharacterMovementComp::Server_RequestSetMaxWalkSpeed_Implementation(const float NewMaxWalkSpeed)
{
	PendingNewMaxWalkSpeed = NewMaxWalkSpeed;
}
#pragma endregion
