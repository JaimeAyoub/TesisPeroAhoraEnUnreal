// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyWavesManager.generated.h"


UCLASS()
class TESISV2_API AEnemyWavesManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AEnemyWavesManager();

	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "EnemyWave")
	UStaticMeshComponent* BeginWall;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "EnemyWave")
	UStaticMeshComponent* EndWall;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "EnemyWave")
	AActor* TriggerZone;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "EnemyWave")
	TArray<ACharacter*> AliveEnemies;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "EnemyWave")
	int EnemiesToSpawn;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "EnemyWave")
	TArray<ATargetPoint*> Spawners;
	
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "EnemyWave")
	TSubclassOf<ACharacter> EnemyReference;
	
	
	
	UFUNCTION(blueprintCallable,Category = "EnemyWave")
	void StartWave();
	UFUNCTION(blueprintCallable,Category = "EnemyWave")
	void DisableWalls();
	UFUNCTION(blueprintCallable,Category = "EnemyWave")
	void EnableWalls();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
