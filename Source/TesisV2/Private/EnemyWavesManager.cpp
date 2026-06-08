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

	BeginWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeginW"));
	BeginWall->SetupAttachment(RootComponent);

	EndWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndW"));
	EndWall->SetupAttachment(RootComponent);
}

void AEnemyWavesManager::StartWave()
{
	if (EnemiesToSpawn > 0 && !Spawners.IsEmpty())
	{
		for (int i = 0; i < EnemiesToSpawn; i++)
		{
			if (EnemyReference != nullptr)
			{
				int indexSpawners = i % Spawners.Num();

				FVector SpawnLocation = Spawners[indexSpawners]->GetTargetLocation();
				FRotator SpawnRotation = FRotator::ZeroRotator;

				ACharacter* NewEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyReference, SpawnLocation, SpawnRotation);
				AliveEnemies.Add(NewEnemy);
			}
		}
	}
}

void AEnemyWavesManager::DisableWalls()
{
	if (BeginWall != nullptr && EndWall != nullptr)
	{
		BeginWall->SetVisibility(false);
		BeginWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		EndWall->SetVisibility(false);
		EndWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemyWavesManager::EnableWalls()
{
	if (BeginWall != nullptr && EndWall != nullptr)
	{
		BeginWall->SetVisibility(true);
		BeginWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		EndWall->SetVisibility(true);
		EndWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

// Called when the game starts or when spawned
void AEnemyWavesManager::BeginPlay()
{
	Super::BeginPlay();

	DisableWalls();
}

// Called every frame
void AEnemyWavesManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
