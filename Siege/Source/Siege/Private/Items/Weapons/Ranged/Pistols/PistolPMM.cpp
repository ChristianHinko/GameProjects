// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolPMM.h"

#include "SiegeCharacter.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"



APistolPMM::APistolPMM()
{
	WeaponInfo.WeaponName = FString("PMM");
	WeaponInfo.WeaponType = EWeaponType::WEAPONTYPE_Pistol;


	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

void APistolPMM::Fire()
{
	Super::Fire();


	SkeletalMesh->PlayAnimation(FireAnim, false);

	UKismetSystemLibrary::PrintString(this, WeaponInfo.WeaponName + FString(" FIRE"));
}

void APistolPMM::OnEquip()
{
	Super::OnEquip();


	
}

void APistolPMM::OnDequip()
{
	Super::OnDequip();


	
}

void APistolPMM::OnAddedToInventory(ASiegeCharacter* OwningCharacter)
{
	Super::OnAddedToInventory(OwningCharacter);


	FAttachmentTransformRules WeaponAttachTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		AttachToComponent(OwningSiegeCharacter->GetMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHandIndex1"));

		SkeletalMesh->SetWorldLocation(SkeletalMesh->GetSocketLocation(TEXT("HandIndex1 Offset")));
		SkeletalMesh->SetRelativeRotation(SkeletalMesh->GetSocketRotation(TEXT("HandIndex1 Offset")));
		SkeletalMesh->SetRelativeScale3D(SkeletalMesh->GetSocketTransform(TEXT("HandIndex1 Offset")).GetScale3D());
	}
	else if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		AttachToComponent(OwningSiegeCharacter->GetSelfMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHandIndex1"));

		SkeletalMesh->SetWorldLocation(SkeletalMesh->GetSocketLocation(TEXT("HandIndex1 Offset")));
		SkeletalMesh->SetRelativeRotation(SkeletalMesh->GetSocketRotation(TEXT("HandIndex1 Offset")));
		SkeletalMesh->SetRelativeScale3D(SkeletalMesh->GetSocketTransform(TEXT("HandIndex1 Offset")).GetScale3D());
	}
}
