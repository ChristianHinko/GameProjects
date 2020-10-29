// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTasks/GAT_Interact.h"

#include "SiegeCharacter.h"
#include "Interactable.h"
#include "Camera/CameraComponent.h"

#include "Kismet/KismetSystemLibrary.h"



UGAT_Interact* UGAT_Interact::CreateInteractTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, ASiegeCharacter* SiegeCharacter)
{
	UGAT_Interact* InteractTask = NewAbilityTask<UGAT_Interact>(OwningAbility, TaskInstanceName);
	InteractTask->SiegeCharacter = SiegeCharacter;
	return InteractTask;
}

void UGAT_Interact::Activate()
{
	if (!Ability)
	{
		OnFailed.Broadcast();
		return;
	}

	if (SiegeCharacter)
	{
		float maxInteractionTraceDistance = 1000.f;

		TArray<FHitResult> InteractSweepHitResults;
		FVector InteractSweepStart = SiegeCharacter->GetPOVCamera()->GetSocketLocation(TEXT("None"));
		FVector InteractSweepEnd = InteractSweepStart + (SiegeCharacter->GetPOVCamera()->GetForwardVector() * maxInteractionTraceDistance);
		FCollisionQueryParams InteractCollisionQueryParams = FCollisionQueryParams(TEXT("InteractTraceCollisionQueryParams"), false, SiegeCharacter);
		if (GetWorld()->SweepMultiByChannel(InteractSweepHitResults, InteractSweepStart, InteractSweepEnd, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(100), InteractCollisionQueryParams))
		{
			for (int i = 0; i < InteractSweepHitResults.Num(); i++)
			{
				AActor* HitActor = InteractSweepHitResults[i].GetActor();


				UKismetSystemLibrary::PrintString(this, GetNameSafe(HitActor));

				IInteractable* HitActorInteractable = Cast<IInteractable>(HitActor);
				if (HitActorInteractable)
				{
					HitActorInteractable->OnInteract();
				}
			}
		}

		OnCompleted.Broadcast();
	}
	else
	{
		OnFailed.Broadcast();
		return;
	}
}
