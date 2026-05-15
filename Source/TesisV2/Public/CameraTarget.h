// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "CameraTarget.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESISV2_API UCameraTarget : public UActorComponent
{
	GENERATED_BODY()

public:
	UCameraTarget();
	//Variables.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=CameraTarget)
	float Radius;
	UPROPERTY(BlueprintReadOnly, Category = CameraTarget)
	AActor* Target;
	UPROPERTY(BlueprintReadOnly, Category = CameraTarget)
	bool IsLock;
	UPROPERTY(BlueprintReadWrite,EditAnywhere ,Category = CameraTarget)
	USpringArmComponent* CameraBoom;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraTarget)
	float DistanceToCancelLock;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraTarget)
	UCameraComponent* Camera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraTarget)
	float LockFov;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = CameraTarget)
	float NormalFov;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = CameraTarget)
	bool isFovChanged;
	

	
	UFUNCTION(BlueprintCallable, Category = CameraTarget)
	void LockCamera();
	UFUNCTION(BlueprintCallable, Category = CameraTarget)
	void SearchEnemy(TArray<AActor*> &Enemies);
	UFUNCTION(Category = CameraTarget)
	void SetRotationOverEnemy(AActor* newTarget,float DeltaTime);
	UFUNCTION(Category = CameraTarget)
	void CheckDistance();
	UFUNCTION(BlueprintCallable,Category = CameraTarget)
	void UnLock();
	
	UFUNCTION(Category = CameraTarget)
	void ChangeFOV(float DeltaTime);
	
	void StartChangeFOV();
	
	void SetMarkEnemy();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
