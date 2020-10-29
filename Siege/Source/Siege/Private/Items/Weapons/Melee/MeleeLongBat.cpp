// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeLongBat.h"

#include "SiegeCharacter.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetSystemLibrary.h"



AMeleeLongBat::AMeleeLongBat()
{
	WeaponInfo.WeaponName = FString("Long Bat");
	WeaponInfo.WeaponType = EWeaponType::WEAPONTYPE_MeleeBig;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaicMesh"));
	RootComponent = StaticMesh;
}

void AMeleeLongBat::Fire()
{
	Super::Fire();


	UKismetSystemLibrary::PrintString(this, WeaponInfo.WeaponName + FString(" FIRE"));
}

void AMeleeLongBat::OnEquip()
{
	Super::OnEquip();


	
}

void AMeleeLongBat::OnDequip()
{
	Super::OnDequip();


	
}

void AMeleeLongBat::OnAddedToInventory(ASiegeCharacter* OwningCharacter)
{
	Super::OnAddedToInventory(OwningCharacter);


	FAttachmentTransformRules WeaponAttachTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);

	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		AttachToComponent(OwningSiegeCharacter->GetMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHandIndex1"));
		SetActorRelativeScale3D(FVector(0.9f, 0.9f, 0.9f));
	}
	else if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		AttachToComponent(OwningSiegeCharacter->GetSelfMesh(), WeaponAttachTransformRules, TEXT("mixamorig_RightHandIndex1"));
	}
}
