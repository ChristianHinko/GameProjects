// Fill out your copyright notice in the Description page of Project Settings.


#include "Objective.h"

#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "SeigeCharacter.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/MaterialBillboardComponent.h"

// Sets default values
AObjective::AObjective()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Objective Mesh"));
	ObjectiveMesh->SetSimulatePhysics(true);
	ObjectiveMesh->SetNotifyRigidBodyCollision(true);
	ObjectiveMesh->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	ObjectiveMesh->OnComponentHit.AddDynamic(this, &AObjective::OnCompHit);

	RootComponent = ObjectiveMesh;
}

// Called when the game starts or when spawned
void AObjective::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if(!Character)
	{
		if(UGameplayStatics::GetPlayerController(this, 0)->GetPawn() != NULL)
		{
			Character = UGameplayStatics::GetPlayerCharacter(this, 0);
			PrimaryActorTick.bCanEverTick = false;
		}
	}
}

void AObjective::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if((Character != NULL) && (OtherActor != NULL) && (OtherActor != this))
	{
		if(OtherActor == Character)
		{
			Collect();
		}
	}
}

void AObjective::Collect()
{
	ObjectiveMesh->SetVisibility(false);
	ObjectiveMesh->SetSimulatePhysics(false);
	ObjectiveMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UMaterialBillboardComponent* billboard;
	billboard = Cast<UMaterialBillboardComponent>(GetComponentByClass(UMaterialBillboardComponent::StaticClass()));
	billboard->SetVisibility(false);
}