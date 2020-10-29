// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttributeSets/AS_Barricade.h"
#include "Interactable.h"

#include "Barricade.generated.h"


class UBoxComponent;
class USkeletalMeshComponent;
class UBarricadeAnimInstance;



UCLASS()
class SIEGE_API ABarricade : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly)
		UBoxComponent* BarricadeVolume;

	UPROPERTY(VisibleDefaultsOnly)
		USkeletalMeshComponent* BarricadeMesh;
	
public:	
	// Sets default values for this actor's properties
	ABarricade();

	// public protected GETTERS //////////
	UBoxComponent* GetBarricadeVolume() const { return BarricadeVolume; }
	USkeletalMeshComponent* GetBarricadeMesh() const { return BarricadeMesh; }

	UAS_Barricade* GetBarricadeAttributeSet() const { return BarricadeAttributeSet; }
	//////////////////////////////////////

	UPROPERTY(EditDefaultsOnly)
		bool bIsBuilt = true;

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
		virtual void OnInteract() override;


	UAS_Barricade* BarricadeAttributeSet;

	UBarricadeAnimInstance* BarricadeAnimationInstance;
};
