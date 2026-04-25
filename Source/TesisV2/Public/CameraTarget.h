// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraTarget.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESISV2_API UCameraTarget : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraTarget();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CameraTarget)
	float Radius;
	UPROPERTY(BlueprintReadOnly, Category = CameraTarget)
	AActor* Target;
	UPROPERTY(BlueprintReadOnly, Category = CameraTarget)
	bool IsLock;

	
	UFUNCTION(BlueprintCallable, Category = CameraTarget)
	void LockCamera();
	UFUNCTION(BlueprintCallable, Category = CameraTarget)
	void SearchEnemy(TArray<AActor*> &Enemies);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
