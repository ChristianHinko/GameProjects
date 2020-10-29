// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PropSpawner/PropSpawnerComponent.h"

#include "SpawnManager.generated.h"

UCLASS()
class GRAVEYARD_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SpawnAllProps();


	TSubclassOf<UPropSpawnerComponent> PropSpawnerComponentTSub;

};
