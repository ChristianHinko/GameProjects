// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SeigeCharacter.h"
#include "SeigeCharacterMovementComponent.h"
#include "SeigeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ASeigeCharacter


ASeigeCharacter::ASeigeCharacter(const class FObjectInitializer& PCIP)
	: Super(PCIP.SetDefaultSubobjectClass<USeigeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	//This will make the Character use your custom MovementComponent class
{
	//TArray<AActor*> SeigeCharacterMovementComponents;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), SeigeCharacterMovementComponent, SeigeCharacterMovementComponents);
	//SeigeCharacterMovementComponent = Cast<USeigeCharacterMovementComponent>(SeigeCharacterMovementComponents[0]);


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(30.f, 80.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Set crouch height
	GetCharacterMovement()->CrouchedHalfHeight = 40.0f;

	// Set input axis dead
	SetDead(0.1f);

	// Set move speeds
	GetCharacterMovement()->MaxWalkSpeed = 250;
	SetMaxRunSpeed(500);
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100;
	//
	walkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// Set walkable floor angle
	GetCharacterMovement()->SetWalkableFloorAngle(60);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetFieldOfView(110);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
}

void ASeigeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SeigeCharacterMovementComponent = Cast<USeigeCharacterMovementComponent>(Super::GetMovementComponent());
}

void ASeigeCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

}
void ASeigeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASeigeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind crouch events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASeigeCharacter::OnCrouch);

	// Bind sprint events
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASeigeCharacter::OnSprint);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASeigeCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASeigeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASeigeCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASeigeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASeigeCharacter::LookUpAtRate);
}

void ASeigeCharacter::OnCrouch()
{
	if(bIsCrouched == false)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ASeigeCharacter::OnSprint()
{
	//SeigeCharacterMovementComponent->SetCustomMovementSubMode(ECustomMovementSubMode::CMSM_None);
	//GetCharacterMovement()->SetMovementMode(MOVE_Custom, SeigeCharacterMovementComponent->CustomMovementSubMode);
	//if(GetCharacterMovement()->MaxWalkSpeed != maxRunSpeed)
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = maxRunSpeed;
	//	if(bIsCrouched == true)
	//	{
	//		UnCrouch();
	//	}
	//}
	//else
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	//}
	if (GetSeigeCharacterMovementComponent()->GetMaxWalkSpeed() == 250)
	{
		GetSeigeCharacterMovementComponent()->SetMaxWalkSpeed(500);
	}
	else
	{
		GetSeigeCharacterMovementComponent()->SetMaxWalkSpeed(250);
	}
}

void ASeigeCharacter::SetMaxRunSpeed(float newmaxRunSpeed)
{
	maxRunSpeed = newmaxRunSpeed;
}

void ASeigeCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ASeigeProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ASeigeCharacter::MoveForward(float Value)
{
	if(UKismetMathLibrary::Abs(Value) >= dead)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		moveForwardIsDead = false;
	}
	else
	{
		moveForwardIsDead = true;
		if(moveRightIsDead == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		}
	}
}

void ASeigeCharacter::MoveRight(float Value)
{
	if(UKismetMathLibrary::Abs(Value) >= dead)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		moveRightIsDead = false;
	}
	else
	{
		moveRightIsDead = true;
		if(moveForwardIsDead == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
		}
	}
}

void ASeigeCharacter::SetDead(float newdead)
{
	dead = newdead;
}

void ASeigeCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASeigeCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
