// Fill out your copyright notice in the Description page of Project Settings.


#include "Barricade.h"

#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BarricadeAnimInstance.h"



// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	BarricadeVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BarricadeVolume"));
	GetBarricadeVolume()->SetupAttachment(GetRootComponent());
	GetBarricadeVolume()->SetRelativeScale3D(FVector(1.75f, 1.5f, 3.5f));
	GetBarricadeVolume()->SetRelativeLocation(FVector(0.f, 0.f, GetBarricadeVolume()->GetRelativeTransform().GetScale3D().Z * 32));

	BarricadeMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BarricadeMesh"));
	GetBarricadeMesh()->SetupAttachment(GetBarricadeVolume());
	GetBarricadeMesh()->SetRelativeScale3D(FVector(1 / 1.75f, 1 / 1.5f, 1 / 3.5f));
	GetBarricadeMesh()->SetRelativeLocation(FVector(0.f, 0.f, -32));
	GetBarricadeMesh()->SetCollisionProfileName(TEXT("BlockAll"));
}
void ABarricade::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	BarricadeAnimationInstance = Cast<UBarricadeAnimInstance>(GetBarricadeMesh()->GetAnimInstance());
	if (bIsBuilt)
	{
		if (BarricadeAnimationInstance)
		{
			BarricadeAnimationInstance->bIsBuilt = false;
		}
	}
	else
	{
		GetBarricadeMesh()->SetHiddenInGame(true);
		GetBarricadeMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABarricade::OnInteract()
{
	bIsBuilt = true;


	GetBarricadeMesh()->SetHiddenInGame(false);
	GetBarricadeMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	if (BarricadeAnimationInstance)
	{
		BarricadeAnimationInstance->bIsBuilt = true;
	}
}
