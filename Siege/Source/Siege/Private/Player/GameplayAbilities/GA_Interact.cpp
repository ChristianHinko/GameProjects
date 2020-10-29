// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Interact.h"

#include "AbilityTasks/GAT_Interact.h"
#include "SiegeCharacter.h"

#include "Kismet/KismetSystemLibrary.h"



bool UGA_Interact::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}
	//////////



	//////////
	return true;
}
void UGA_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		ASiegeCharacter* SiegeCharacter = Cast<ASiegeCharacter>(ActorInfo->AvatarActor);
		if (SiegeCharacter)
		{
			UGAT_Interact* InteractTask = UGAT_Interact::CreateInteractTask(this, FName("InteractProxy"), SiegeCharacter);
			InteractTask->OnCompleted.AddUObject(this, &UGA_Interact::OnTaskCompleted);
			InteractTask->OnFailed.AddUObject(this, &UGA_Interact::OnTaskFailed);
			InteractTask->ReadyForActivation();
		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGA_Interact::OnTaskCompleted()
{
	UKismetSystemLibrary::PrintString(this, "Completed");

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
void UGA_Interact::OnTaskFailed()
{
	UKismetSystemLibrary::PrintString(this, "FAILED");

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}