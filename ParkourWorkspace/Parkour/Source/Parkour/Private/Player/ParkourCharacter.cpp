// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ParkourCharacter.h"

#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/KismetSystemLibrary.h"



AParkourCharacter::AParkourCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;


	// Mesh defaults
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	// Create POVMesh
	POVMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("POVMesh"));
	POVMesh->SetupAttachment(GetCapsuleComponent());
	POVMesh->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	POVMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	POVMesh->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));

	// Create CameraBoom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}


void AParkourCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	//Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AParkourCharacter::OnPressedJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AParkourCharacter::OnReleasedJump);


	//Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AParkourCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AParkourCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AParkourCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AParkourCharacter::LookUpAtRate);
}
#pragma region Input Events
//Actions
void AParkourCharacter::OnPressedJump()
{
	Jump();
}
void AParkourCharacter::OnReleasedJump()
{
	StopJumping();
}

//Axis
void AParkourCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AParkourCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AParkourCharacter::TurnAtRate(float Rate)
{
	if (Rate != 0)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}
void AParkourCharacter::LookUpAtRate(float Rate)
{
	if (Rate != 0)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}
#pragma endregion
