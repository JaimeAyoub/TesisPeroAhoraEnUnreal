// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTarget.h"

#include "VectorTypes.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UCameraTarget::UCameraTarget()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCameraTarget::LockCamera()
{
	isFovChanged = false;
	if (!IsLock)
	{
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TArray<AActor*> ActorsToIgnore;
		TArray<AActor*> OutActors;

		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
		ActorsToIgnore.Add(GetOwner());
		UKismetSystemLibrary::SphereOverlapActors(
			this,
			GetOwner()->GetActorLocation(),
			Radius,
			ObjectTypes,
			nullptr,
			ActorsToIgnore,
			OutActors
		);
		if (OutActors.Num() != 0)
		{
			IsLock = true;
			SearchEnemy(OutActors);
		}
	}
	else
	{
		UnLock();
	}
}

void UCameraTarget::SearchEnemy(TArray<AActor*>& Enemies)
{
	FVector ActualPlayerPos = GetOwner()->GetActorLocation();
	float MinDistance = FLT_MAX;
	AActor* ClosestEnemy = nullptr;
	for (AActor* Actor : Enemies)
	{
		if (FVector::Dist(ActualPlayerPos, Actor->GetActorLocation()) < MinDistance)
		{
			ClosestEnemy = Actor;
			MinDistance = FVector::Dist(ActualPlayerPos, Actor->GetActorLocation());
		}
	}
	Target = ClosestEnemy;
}

void UCameraTarget::SetRotationOverEnemy(AActor* newTarget, float DeltaTime)
{
	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(CameraBoom->GetComponentLocation(),
	                                                      newTarget->GetActorLocation());

	FRotator NewRot = UKismetMathLibrary::RInterpTo(CameraBoom->GetTargetRotation(), Rot, DeltaTime, 5.0f);

	AController* MyController = Cast<APawn>(GetOwner())->GetController();
	if (MyController)
	{
		MyController->SetControlRotation(NewRot);
	}
}

void UCameraTarget::CheckDistance()
{
	if (FVector::Dist(GetOwner()->GetActorLocation(), Target->GetActorLocation()) > DistanceToCancelLock)
	{
		UnLock();
	}
}


void UCameraTarget::UnLock()
{
	Target = nullptr;
	IsLock = false;
}

void UCameraTarget::ChangeFOV(float DeltaTime)
{
	
	if (!isFovChanged)
	{
		float TargetFOV = IsLock ? LockFov : NormalFov;
		float NewFOV = UKismetMathLibrary::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, 5.0f);
		Camera->SetFieldOfView(NewFOV);

		if (FMath::IsNearlyEqual(Camera->FieldOfView, TargetFOV, 0.5f))
		{
			isFovChanged = true;
		}
	}
}

// Called when the game starts
void UCameraTarget::BeginPlay()
{
	Super::BeginPlay();

	CameraBoom = GetOwner()->FindComponentByClass<USpringArmComponent>();
	Radius = 1000.0f;
	DistanceToCancelLock = 2000.0f;
	NormalFov = 90.0f;
	LockFov = 80.0f;
}


// Called every frame
void UCameraTarget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChangeFOV(DeltaTime);
	if (IsLock && CameraBoom)
	{
		if (Target != nullptr)
		{
			SetRotationOverEnemy(Target, DeltaTime);
			CheckDistance();
		}
	}
}
