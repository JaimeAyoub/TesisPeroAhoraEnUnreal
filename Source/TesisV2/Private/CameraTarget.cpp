// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTarget.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UCameraTarget::UCameraTarget()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCameraTarget::LockCamera()
{
	if (!IsLock)
	{
		IsLock = true;
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
			SearchEnemy(OutActors);
		}
	}
	else
	{
		Target = nullptr;
		IsLock = false;
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

// Called when the game starts
void UCameraTarget::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UCameraTarget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
