// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunSASG12.h"

#include "SiegeCharacter.h"
#include "Components/SkeletalMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"



AShotgunSASG12::AShotgunSASG12()
{
	WeaponInfo.WeaponName = FString("SASG-12");
	WeaponInfo.WeaponType = EWeaponType::WEAPONTYPE_Shotgun;


	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

void AShotgunSASG12::Fire()
{
	Super::Fire();


	SkeletalMesh->PlayAnimation(FireAnim, false);

	UKismetSystemLibrary::PrintString(this, WeaponInfo.WeaponName + FString(" FIRE"));
}

void AShotgunSASG12::OnEquip()
{
	Super::OnEquip();


	
}

void AShotgunSASG12::OnDequip()
{
	Super::OnDequip();


	
}

void AShotgunSASG12::OnAddedToInventory(ASiegeCharacter* OwningCharacter)
{
	Super::OnAddedToInventory(OwningCharacter);


	FAttachmentTransformRules WeaponAttachTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		AttachToComponent(OwningSiegeCharacter->GetMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHandIndex1"));
		SetActorRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));
	}
	else if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		AttachToComponent(OwningSiegeCharacter->GetSelfMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHandIndex1"));
		UKismetSystemLibrary::PrintString(this, "Autonomous attachment");
	}
}
