#include "FGInteractableLineTraceComponent.h"

#include "DrawDebugHelpers.h"
#include "FGPlayer.h"
#include "Camera/CameraComponent.h"
#include "GP3_Team02/Crain/CrainButton.h"
#include "GP3_Team02/Interactables/FGInteractable.h"
#include "GP3_Team02/Interactables/FGPickupInteractable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogInteractionSystem);

UFGInteractableLineTraceComponent::UFGInteractableLineTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFGInteractableLineTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	PlayerCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
}

void UFGInteractableLineTraceComponent::DropPickupable()
{
	if (CurrentlyHeldInteractable != nullptr)
	{
		CurrentlyHeldInteractable->OnDrop();
		CurrentlyHeldInteractable = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Tried to drop pickupable even though the player isn't holding a pickupable"));
	}
}

void UFGInteractableLineTraceComponent::PickupInteractable(AFGPickupInteractable* Pickup)
{
	if (Pickup == nullptr)
		return;

	if (CurrentlyHeldInteractable == nullptr)
	{
		CurrentlyHeldInteractable = Pickup;
		BP_OnPickupInteractable.Broadcast(Pickup);
	}
}

void UFGInteractableLineTraceComponent::StartInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("StartInteract : %i"), bIsInteracting)
	bIsInteracting = true;
	HandleInteract();
}

void UFGInteractableLineTraceComponent::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("EndInteract : %i"), bIsInteracting)
	bIsInteracting = false;
}

void UFGInteractableLineTraceComponent::HandleInteract()
{
	if (CurrentlyHeldInteractable != nullptr)
	{
		DropPickupable();
		return;
	}

	FHitResult Hit;
	FVector Start = PlayerCamera->GetComponentLocation();
	FVector End = Start + PlayerCamera->GetForwardVector() * RaycastRange;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel))
	{
		AFGInteractable* Interactable = Cast<AFGInteractable>(Hit.GetActor());

		if (Interactable != nullptr)
		{
			Interactable->Interact(this);
			UE_LOG(LogInteractionSystem, Log, TEXT("Interacted with interactable with name: %s"),
			       *Interactable->GetName());
		}
	}
}

void UFGInteractableLineTraceComponent::HoldHandleInteract()
{
	if (bIsInteracting == false) return;

	FHitResult Hit;
	FVector Start = PlayerCamera->GetComponentLocation();
	FVector End = Start + PlayerCamera->GetForwardVector() * RaycastRange;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel))
	{
		ACrainButton* Interactable = Cast<ACrainButton>(Hit.GetActor());

		if (Interactable != nullptr)
		{
			Interactable->Interact(this);
			UE_LOG(LogInteractionSystem, Log, TEXT("Interacted with interactable with name: %s"),
			       *Interactable->GetName());
		}
	}
}

void UFGInteractableLineTraceComponent::TurnOnInteractableFeedback(AFGInteractable* Interactable)
{
	UE_LOG(LogInteractionSystem, Log, TEXT("Turning on interactable feedback"));

	OutlinedInteractable = Interactable;
	BP_OnInteractableFeedback.Broadcast(Interactable, true);
	UStaticMeshComponent* Mesh = Interactable->FindComponentByClass<UStaticMeshComponent>();

	if (Mesh != nullptr)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void UFGInteractableLineTraceComponent::TurnOffInteractableFeedback()
{
	UE_LOG(LogInteractionSystem, Log, TEXT("Turning off interactable feedback"));

	BP_OnInteractableFeedback.Broadcast(OutlinedInteractable, false);
	UStaticMeshComponent* Mesh = OutlinedInteractable->FindComponentByClass<UStaticMeshComponent>();

	if (Mesh != nullptr)
	{
		Mesh->SetRenderCustomDepth(false);
	}

	OutlinedInteractable = nullptr;
}

bool UFGInteractableLineTraceComponent::SortFHitResult(FHitResult A, FHitResult B)
{
	if (A.Time < B.Time)
		return true;

	return false;
}

void UFGInteractableLineTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HoldHandleInteract();

	if (DebugTrace)
	{
		FHitResult Hit;
		FVector Start = PlayerCamera->GetComponentLocation();
		FVector End = Start + PlayerCamera->GetForwardVector() * RaycastRange;

		UKismetSystemLibrary::DrawDebugLine(this, Start, End, TraceDebugColor);
	}

	if (CurrentlyHeldInteractable == nullptr)
	{
		FHitResult Hit;
		FVector Start = PlayerCamera->GetComponentLocation();
		FVector End = Start + PlayerCamera->GetForwardVector() * RaycastRange;

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel))
		{
			AFGInteractable* Interactable = Cast<AFGInteractable>(Hit.GetActor());

			if (Interactable != nullptr)
			{
				if (OutlinedInteractable == nullptr)
				{
					TurnOnInteractableFeedback(Interactable);
					return;
				}
				if (OutlinedInteractable != Interactable)
				{
					TurnOffInteractableFeedback();
					TurnOnInteractableFeedback(Interactable);
					return;
				}
			}
			else if(OutlinedInteractable != nullptr)
			{
				TurnOffInteractableFeedback();
			}
		}
		else if (OutlinedInteractable != nullptr)
		{
			TurnOffInteractableFeedback();
		}
		return;
	}

	if (OutlinedInteractable != nullptr)
	{
		TurnOffInteractableFeedback();
	}

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CurrentlyHeldInteractable);

	FHitResult HitResult;
	FVector StartTrace = CurrentlyHeldInteractable->GetActorLocation() + FVector::UpVector * 10;
	FVector EndTrace = StartTrace + FVector::UpVector * 100;

	//Try sweep if it nosn't exist then use box cheack
	GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Pawn, QueryParams);
	if (HitResult.bBlockingHit)
	{
		AFGPlayer* Player = Cast<AFGPlayer>(HitResult.Actor);

		if (Player)
		{
			DropPickupable();
			return;
		}
	}	

	UCameraComponent* CameraComponent = GetOwner()->FindComponentByClass<UCameraComponent>();
	const FVector CameraPosition = CameraComponent->GetComponentLocation();
	const FVector CameraForward = CameraComponent->GetForwardVector();
	const FVector Forward = GetOwner()->GetActorForwardVector();

	const float PitchAngle = FMath::Acos(FVector::DotProduct(CameraForward, Forward));
	float HeightOffset = FMath::Tan(PitchAngle) * HeldInteractableDistance;
	const FVector OffsetVector = CameraForward.Z < 0 ? -FVector::UpVector : FVector::UpVector;

	HeightOffset = FMath::Clamp(HeightOffset, 0.f, MaxHeightOffset);

	FVector DirectionParallelToCamForward;

	if(CurrentlyHeldInteractable->RotateAroundAxisWhenHeld)
	{
		DirectionParallelToCamForward = Forward.RotateAngleAxis(CurrentlyHeldInteractable->AxisRotationDegrees, CurrentlyHeldInteractable->Axis);
	}
	else
		DirectionParallelToCamForward = Forward;

	CurrentlyHeldInteractable->SetActorRotation(UKismetMathLibrary::MakeRotFromXZ(DirectionParallelToCamForward, FVector::UpVector));

	const FVector MeshPos = CurrentlyHeldInteractable->Mesh->GetComponentLocation();
	const FVector TargetLocation = (CameraPosition + Forward * HeldInteractableDistance + HeightOffset * OffsetVector) + CurrentlyHeldInteractable->PickupPivotOffset;
	FVector CurrentTargetDelta = TargetLocation - MeshPos;

	const float SqrDistance = CurrentTargetDelta.SizeSquared();

	CurrentlyHeldInteractable->HeldVelocity = CurrentTargetDelta / DeltaTime;

	FHitResult Hit;

	for (int i = 0; i < MovementIterativeDepth; i++)
	{
		if (CurrentlyHeldInteractable == nullptr)
		{
			return;
		}
		CurrentlyHeldInteractable->AddActorWorldOffset(CurrentTargetDelta, true, &Hit);

		CurrentTargetDelta -= CurrentTargetDelta * Hit.Time;

		if (Hit.bBlockingHit)
		{
			if (Hit.bStartPenetrating)
			{
				UE_LOG(LogInteractionSystem, Log, TEXT("Depenetrating"));
				CurrentlyHeldInteractable->AddActorWorldOffset(Hit.PenetrationDepth * Hit.Normal * 1.1f);
				break;
			}

			AActor* HitActor = Hit.GetActor();

			if (HitActor == nullptr)
				return;

			if (HitActor->IsRootComponentMovable() && HitActor->GetRootComponent()->IsSimulatingPhysics() && CurrentlyHeldInteractable != nullptr)
			{
				UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
				if (Primitive != nullptr)
				{
					Primitive->SetPhysicsLinearVelocity(
						(CurrentlyHeldInteractable->HeldVelocity / Primitive->GetMass()) * CollisionForceMultiplier);
					continue;
				}
			}

			const FVector DeprenetationDelta = FVector::DotProduct(Hit.Normal, CurrentTargetDelta) * Hit.Normal;
			CurrentTargetDelta -= DeprenetationDelta;

			if (CurrentTargetDelta.IsNearlyZero())
				break;

			CurrentTargetDelta -= FVector::DotProduct(CurrentTargetDelta, Hit.Normal) * Hit.Normal;
		}
	}

	if (SqrDistance > FMath::Square(MaxDistance))
	{
		DropPickupable();
	}
}
