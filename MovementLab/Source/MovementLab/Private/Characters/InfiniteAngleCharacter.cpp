// Fill out your copyright notice in the Description page of Project Settings.


#include "InfiniteAngleCharacter.h"

#include "MovementLabCharacterMovementComp.h"
#include "Kismet\KismetSystemLibrary.h"



AInfiniteAngleCharacter::AInfiniteAngleCharacter(const FObjectInitializer& PCIP)
	: Super(PCIP.SetDefaultSubobjectClass<UMovementLabCharacterMovementComp>(ACharacter::CharacterMovementComponentName))
{
	
}
void AInfiniteAngleCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	MovementLabCharacterMovementComp = Cast<UMovementLabCharacterMovementComp>(GetMovementComponent());
}

void AInfiniteAngleCharacter::BeginPlay()
{
	Super::BeginPlay();


}



void AInfiniteAngleCharacter::JumpPressed()
{
	Super::JumpPressed();


}
void AInfiniteAngleCharacter::JumpReleased()
{
	Super::JumpReleased();


}

void AInfiniteAngleCharacter::SprintPressed()
{
	Super::SprintPressed();


	GetCharacterMovementComponent()->RequestSetMaxWalkSpeed(RunSpeed);
}
void AInfiniteAngleCharacter::SprintReleased()
{
	Super::SprintReleased();


	GetCharacterMovementComponent()->RequestSetMaxWalkSpeed(WalkSpeed);
}

void AInfiniteAngleCharacter::InteractPressed()
{
	Super::InteractPressed();


	UKismetSystemLibrary::PrintString(this, "InteractPressed");
	GetCharacterMovementComponent()->RequestSetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_InfiniteWalk);
}
void AInfiniteAngleCharacter::InteractReleased()
{
	Super::InteractReleased();


	UKismetSystemLibrary::PrintString(this, "InteractReleased");
	GetCharacterMovementComponent()->RequestSetMovementMode(EMovementMode::MOVE_Walking);
}


void AInfiniteAngleCharacter::MoveForward(float Value)
{
	Super::MoveForward(Value);


}
void AInfiniteAngleCharacter::MoveRight(float Value)
{
	Super::MoveRight(Value);


}

void AInfiniteAngleCharacter::TurnAtRate(float Rate)
{
	Super::TurnAtRate(Rate);


}
void AInfiniteAngleCharacter::LookUpAtRate(float Rate)
{
	Super::LookUpAtRate(Rate);


}
