// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWavesManager.h"

#include "Engine/StaticMeshActor.h"
#include "GameFramework/Character.h"

// Sets default values
AEnemyWavesManager::AEnemyWavesManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void AEnemyWavesManager::StartWave()
{
	if (EnemiesToSpawn > 0 && !Spawners.IsEmpty())
	{
		int indexSpawners = 0;
		for (int i = 0; i < EnemiesToSpawn; i++)
		{
			if (EnemyReference != nullptr)
			{
				if (indexSpawners > EnemiesToSpawn)
					indexSpawners = 0;
				FVector SpawnLocation = Spawners[indexSpawners]->GetActorLocation();
				FRotator SpawnRotation = FRotator::ZeroRotator;

				ACharacter* NewEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyReference, SpawnLocation, SpawnRotation);
			}
		}
	}
}

// Called when the game starts or when spawned
void AEnemyWavesManager::BeginPlay()
{
	Super::BeginPlay();
	if (BeginWall != nullptr && EndWall != nullptr)
	{
		BeginWall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		EndWall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void AEnemyWavesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
