// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTarget.h"

#include "VectorTypes.h"
#include "Components/BillboardComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UCameraTarget::UCameraTarget()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCameraTarget::LockCamera()
{
	StartChangeFOV();
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
		if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
		{
			OwnerChar->GetCharacterMovement()->bOrientRotationToMovement = false;
			OwnerChar->bUseControllerRotationYaw = true;
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
	SetMarkEnemy();
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
	SetMarkEnemy();
	Target = nullptr;
	IsLock = false;
	StartChangeFOV();
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		OwnerChar->GetCharacterMovement()->bOrientRotationToMovement = true;
		OwnerChar->bUseControllerRotationYaw = false;
	}
}

void UCameraTarget::ChangeFOV(float DeltaTime)
{
	float TargetFOV;
	UE_LOG(LogTemp, Warning, TEXT("isFovChanged: %d | isSprinting: %d"), isFovChanged, isSprinting);
	if (!isFovChanged && Camera)
	{
		if (isSprinting)
			FovToChange = 100.0f;

		if (IsLock)
			FovToChange = 80.0f;
		if (IsLock || isSprinting)
		{
			TargetFOV = FovToChange;
		}
		else
		{
			TargetFOV = NormalFov;
		}
		float NewFOV = UKismetMathLibrary::FInterpTo(Camera->FieldOfView, TargetFOV, DeltaTime, 8.0f);
		Camera->SetFieldOfView(NewFOV);

		if (FMath::IsNearlyEqual(Camera->FieldOfView, TargetFOV, 0.5f))
		{
			isFovChanged = true;
		}
	}
}

void UCameraTarget::StartChangeFOV()
{
	isFovChanged = false;
}

void UCameraTarget::SetMarkEnemy()
{
	UPaperSpriteComponent* Sprite;
	if (Target != nullptr)
	{
		Sprite = Target->GetComponentByClass<UPaperSpriteComponent>();
		if (Sprite != nullptr)
		{
			if (Sprite->IsVisible())
			{
				Sprite->SetVisibility(false);
			}
			else
			{
				Sprite->SetVisibility(true);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No hay sprite"));
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
	StartChangeFOV();
}


// Called every frame
void UCameraTarget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Camera != nullptr)
	{
		ChangeFOV(DeltaTime);
	}
	if (IsLock && CameraBoom)
	{
		if (Target != nullptr)
		{
			SetRotationOverEnemy(Target, DeltaTime);
			CheckDistance();
		}
	}
}
