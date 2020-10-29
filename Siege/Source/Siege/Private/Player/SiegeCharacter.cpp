// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SiegeCharacter.h"

#include "UnrealNetwork.h"
#include "GameplayAbility.h"
#include "SiegeCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GA_SetWeapon.h"
#include "CharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "SiegePlayerState.h"
#include "GameFramework/InputSettings.h"
#include "WeaponInventory.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"



void ASiegeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASiegeCharacter, FireAbilitySpecHandle, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASiegeCharacter, SwitchWeaponAbilitySpecHandle, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASiegeCharacter, SetWeaponAbilitySpecHandle, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ASiegeCharacter, InteractAbilitySpecHandle, COND_OwnerOnly);
	DOREPLIFETIME(ASiegeCharacter, ControllerRotation);
}


ASiegeCharacter::ASiegeCharacter(const FObjectInitializer& PCIP)
	: Super(PCIP.SetDefaultSubobjectClass<USiegeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	CharacterAttributeSet = CreateDefaultSubobject<UCharacterAttributeSet>(TEXT("CharacterAttributeSet"));

	// Mesh defaults
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetRelativeLocation(FVector(-20.f, 0.f, -96.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(2.25f, 2.25f, 2.25f));

	// SelfMesh and defaults
	SelfMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SelfMesh"));
	SelfMesh->SetOnlyOwnerSee(true);
	SelfMesh->SetupAttachment(GetCapsuleComponent());
	SelfMesh->bCastDynamicShadow = false;
	SelfMesh->CastShadow = false;
	SelfMesh->SetRelativeLocation(FVector(-20.f, 0.f, -96.f));
	SelfMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SelfMesh->SetRelativeScale3D(FVector(2.25f, 2.25f, 2.25f));

	// POVCamera and defaults
	POVCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	POVCamera->SetupAttachment(GetCapsuleComponent());
	POVCamera->SetRelativeLocation(FVector(0.f, 10.f, 70.f));
	POVCamera->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	POVCamera->bUsePawnControlRotation = true;
	POVCamera->SetFieldOfView(110);

	// WeaponInventory and defaults
	WeaponInventoryComponent = CreateDefaultSubobject<UWeaponInventory>(TEXT("WeaponInventory"));

}
void ASiegeCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	SiegeCharacterMovementComponent = Cast<USiegeCharacterMovementComponent>(Super::GetMovementComponent());

	// POVCamera attach to parent socket
	POVCamera->AttachToComponent(SelfMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false), TEXT("POVCamera_Socket"));
}

void ASiegeCharacter::BeginPlay()
{
	Super::BeginPlay();


}

void ASiegeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


}

void ASiegeCharacter::OnPossessedByClient(AController* NewController)
{
	ServerSpawnDefaultInventory();
}

void ASiegeCharacter::OnAbilityDataInjected()
{
	if (HasAuthority() && GetAbilitySystemComponent())
	{
		FireAbilitySpecHandle = GetAbilitySystemComponent()->GrantAbility(this, FireAbility);
		SwitchWeaponAbilitySpecHandle = GetAbilitySystemComponent()->GrantAbility(this, SwitchWeaponAbility);
		SetWeaponAbilitySpecHandle = GetAbilitySystemComponent()->GrantAbility(this, SetWeaponAbility);
		InteractAbilitySpecHandle = GetAbilitySystemComponent()->GrantAbility(this, InteractAbility);
    }
}

bool ASiegeCharacter::ServerSpawnDefaultInventory_Validate()
{
	return true;
}
void ASiegeCharacter::ServerSpawnDefaultInventory_Implementation()
{
	GetWeaponInventoryComponent()->SpawnDefaultInventory();
}

void ASiegeCharacter::OnHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{

}

void ASiegeCharacter::OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{

}

void ASiegeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();


	ASiegePlayerState* SiegePlayerState = GetPlayerState<ASiegePlayerState>();
	if (SiegePlayerState)
	{
		SiegeAbilitySystemComponent = Cast<USiegeAbilitySystemComponent>(SiegePlayerState->GetAbilitySystemComponent());
		SiegeAbilitySystemComponent->RefreshAbilityActorInfo();
	}
}


void ASiegeCharacter::Tick(float DeltaTimeX)
{
	Super::Tick(DeltaTimeX);


	if (HasAuthority())
	{
		ControllerRotation = GetControlRotation();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASiegeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// Action
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASiegeCharacter::OnJumpPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASiegeCharacter::OnJumpReleased);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASiegeCharacter::OnCrouchPressed);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASiegeCharacter::OnCrouchReleased);

	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &ASiegeCharacter::OnPronePressed);
	PlayerInputComponent->BindAction("Prone", IE_Released, this, &ASiegeCharacter::OnProneReleased);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASiegeCharacter::OnFirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASiegeCharacter::OnFireReleased);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASiegeCharacter::OnReloadPressed);
	PlayerInputComponent->BindAction("Reload", IE_Released, this, &ASiegeCharacter::OnReloadReleased);

	PlayerInputComponent->BindAction("AimDownSights", IE_Pressed, this, &ASiegeCharacter::OnADSPressed);
	PlayerInputComponent->BindAction("AimDownSights", IE_Released, this, &ASiegeCharacter::OnADSReleased);

	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ASiegeCharacter::OnSwitchWeaponPressed);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Released, this, &ASiegeCharacter::OnSwitchWeaponReleased);

	PlayerInputComponent->BindAction("TacticalMelee", IE_Pressed, this, &ASiegeCharacter::OnTacticalMeleePressed);
	PlayerInputComponent->BindAction("TacticalMelee", IE_Released, this, &ASiegeCharacter::OnTacticalMeleeReleased);

	PlayerInputComponent->BindAction("WeaponSlot1", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot1Pressed);
	PlayerInputComponent->BindAction("WeaponSlot1", IE_Released, this, &ASiegeCharacter::OnWeaponSlot1Released);

	PlayerInputComponent->BindAction("WeaponSlot2", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot2Pressed);
	PlayerInputComponent->BindAction("WeaponSlot2", IE_Released, this, &ASiegeCharacter::OnWeaponSlot2Released);

	PlayerInputComponent->BindAction("WeaponSlot3", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot3Pressed);
	PlayerInputComponent->BindAction("WeaponSlot3", IE_Released, this, &ASiegeCharacter::OnWeaponSlot3Released);

	PlayerInputComponent->BindAction("WeaponSlot4", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot4Pressed);
	PlayerInputComponent->BindAction("WeaponSlot4", IE_Released, this, &ASiegeCharacter::OnWeaponSlot4Released);

	PlayerInputComponent->BindAction("WeaponSlot5", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot5Pressed);
	PlayerInputComponent->BindAction("WeaponSlot5", IE_Released, this, &ASiegeCharacter::OnWeaponSlot5Released);

	PlayerInputComponent->BindAction("WeaponSlot6", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot6Pressed);
	PlayerInputComponent->BindAction("WeaponSlot6", IE_Released, this, &ASiegeCharacter::OnWeaponSlot6Released);

	PlayerInputComponent->BindAction("WeaponSlot7", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot7Pressed);
	PlayerInputComponent->BindAction("WeaponSlot7", IE_Released, this, &ASiegeCharacter::OnWeaponSlot7Released);

	PlayerInputComponent->BindAction("WeaponSlot8", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot8Pressed);
	PlayerInputComponent->BindAction("WeaponSlot8", IE_Released, this, &ASiegeCharacter::OnWeaponSlot8Released);

	PlayerInputComponent->BindAction("WeaponSlot9", IE_Pressed, this, &ASiegeCharacter::OnWeaponSlot9Pressed);
	PlayerInputComponent->BindAction("WeaponSlot9", IE_Released, this, &ASiegeCharacter::OnWeaponSlot9Released);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASiegeCharacter::OnInteractPressed);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ASiegeCharacter::OnInteractReleased);

	// Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ASiegeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASiegeCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASiegeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASiegeCharacter::LookUpAtRate);
}

void ASiegeCharacter::OnJumpPressed()
{
	ACharacter::Jump();
}
void ASiegeCharacter::OnJumpReleased()
{
	ACharacter::StopJumping();
}

void ASiegeCharacter::OnCrouchPressed()
{
	
}
void ASiegeCharacter::OnCrouchReleased()
{
	
}

void ASiegeCharacter::OnPronePressed()
{

}
void ASiegeCharacter::OnProneReleased()
{
	
}

void ASiegeCharacter::OnFirePressed()
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(FireAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnFireReleased()
{
	
}

void ASiegeCharacter::OnReloadPressed()
{
	
}
void ASiegeCharacter::OnReloadReleased()
{
	
}

void ASiegeCharacter::OnADSPressed()
{

}
void ASiegeCharacter::OnADSReleased()
{
	
}

void ASiegeCharacter::OnSwitchWeaponPressed()
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SwitchWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnSwitchWeaponReleased()
{
	
}

void ASiegeCharacter::OnTacticalMeleePressed()
{

}
void ASiegeCharacter::OnTacticalMeleeReleased()
{

}

void ASiegeCharacter::OnWeaponSlot1Pressed()
{
	weaponSlot = 0;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot1Released()
{

}

void ASiegeCharacter::OnWeaponSlot2Pressed()
{
	weaponSlot = 1;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot2Released()
{

}

void ASiegeCharacter::OnWeaponSlot3Pressed()
{
	weaponSlot = 2;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot3Released()
{

}

void ASiegeCharacter::OnWeaponSlot4Pressed()
{
	weaponSlot = 3;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot4Released()
{

}

void ASiegeCharacter::OnWeaponSlot5Pressed()
{
	weaponSlot = 4;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot5Released()
{

}

void ASiegeCharacter::OnWeaponSlot6Pressed()
{
	weaponSlot = 5;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot6Released()
{

}

void ASiegeCharacter::OnWeaponSlot7Pressed()
{
	weaponSlot = 6;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot7Released()
{

}

void ASiegeCharacter::OnWeaponSlot8Pressed()
{
	weaponSlot = 7;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot8Released()
{

}

void ASiegeCharacter::OnWeaponSlot9Pressed()
{
	weaponSlot = 8;
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(SetWeaponAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnWeaponSlot9Released()
{

}

void ASiegeCharacter::OnInteractPressed()
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->TryActivateAbility(InteractAbilitySpecHandle, true);
	}
}
void ASiegeCharacter::OnInteractReleased()
{

}



void ASiegeCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASiegeCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASiegeCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASiegeCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
