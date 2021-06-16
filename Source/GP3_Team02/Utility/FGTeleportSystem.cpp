#include "FGTeleportSystem.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GP3_Team02/Character/FGInteractableLineTraceComponent.h"
#include "GP3_Team02/Character/FGPlayer.h"
#include "GP3_Team02/Interactables/FGPickupInteractable.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTeleportSystem);

AFGTeleportSystem::AFGTeleportSystem()
{
	TeleportBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("Teleportation collider"));
	RootComponent = TeleportBounds;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Teleport Location"));
	SceneComponent->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AFGTeleportSystem::BeginPlay()
{
	Super::BeginPlay();

	TeleportBounds->OnComponentBeginOverlap.AddDynamic(this, &AFGTeleportSystem::HandleOverlap);
}

void AFGTeleportSystem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float Time = GetGameTimeSinceCreation();

	if(TeleportQueued)
	{
		if(Time - TimeOfQueue > DelayBeforeActualTeleport)
		{
			TeleportQueued = false;
			
			if(!Teleport(SavedData))
			{
				UE_LOG(LogTeleportSystem, Warning, TEXT("Teleport back failed, see above log message for more info"));
				return;
			}
			
			if(!TeleportingBack)
			{
				SetActorTickEnabled(false);
			}

			if(!DontUpdateDestinationOnSweep)
				SceneComponent->SetRelativeLocation(SceneComponent->GetRelativeLocation() + SavedData.SweepDelta);
			
			FVector Relative = SceneComponent->GetRelativeLocation();
			UE_LOG(LogTeleportSystem, Log, TEXT("New scene component position (relative): x: %f, y: %f, z: %f"), Relative.X, Relative.Y, Relative.Z);
		}
		return;
	}

	if (bTeleportBack && TeleportingBack && Time - TimeOfTeleport > TeleportBackDelay)
	{
		FVector Destination;
		TeleportingBack = false;
		
		if(bRelativeDestination)
			Destination = -SceneComponent->GetRelativeLocation();
		else
			Destination = LastPosition;

		const FTeleportData Data = FTeleportData(Cast<AFGPlayer>(UGameplayStatics::GetPlayerPawn(this, 0)), Destination, bRelativeDestination);
		QueueTeleport(Data);
	}
}

void AFGTeleportSystem::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	AFGPlayer* Player = Cast<AFGPlayer>(OtherActor);
	if(Player == nullptr)
		return;
	
	const FTeleportData Data = FTeleportData(Player, SceneComponent->GetRelativeLocation(), bRelativeDestination);
	QueueTeleport(Data);

	if(bTeleportBack)
		TeleportingBack = true;
}

void AFGTeleportSystem::QueueTeleport(const FTeleportData& Data)
{
	TeleportQueued = true;
	SavedData = Data;
	TimeOfQueue = GetGameTimeSinceCreation();
	SetActorTickEnabled(true);
}

bool AFGTeleportSystem::Teleport(FTeleportData& Data)
{
	UE_LOG(LogTeleportSystem, Log, TEXT("Teleport method called"));

	if(Data.Player == nullptr)
	{
		Data.Player = Cast<AFGPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	
	if (Data.Player != nullptr)
	{
		UFGInteractableLineTraceComponent* LineTrace = Data.Player->FindComponentByClass<UFGInteractableLineTraceComponent>();

		FVector CurrentDestination;

		FTransform PlayerTransform = Data.Player->GetTransform();
		FTransform TeleportTransform = GetTransform();

		if (Data.bDestinationIsRelative)
			CurrentDestination = PlayerTransform.TransformPositionNoScale(Data.DestinationPosition);
		else
			CurrentDestination = TeleportTransform.TransformPositionNoScale(Data.DestinationPosition);

		FVector PlayerPos = Data.Player->GetActorLocation();
		UE_LOG(LogTeleportSystem, Log, TEXT("Destination Position: x: %f, y: %f, z: %f, Destination to teleport to: x: %f, y: %f, z: %f, Player location: x: %f, y: %f, z: %f"), Data.DestinationPosition.X, Data.DestinationPosition.Y, Data.DestinationPosition.Z, CurrentDestination.X, CurrentDestination.Y, CurrentDestination.Z, PlayerPos.X, PlayerPos.Y, PlayerPos.Z);

		if (bSweepToFloor)
		{
			FHitResult Hit;
			FCollisionShape CapsuleCollision;
			CapsuleCollision.SetCapsule(Data.Player->FindComponentByClass<UCapsuleComponent>()->Bounds.BoxExtent);

			const FName TraceTag("TeleportSystemSweep");
			
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
			GetWorld()->DebugDrawTraceTag = TraceTag;
#endif

			FCollisionQueryParams CollisionParams;

			if (bDebugTrace)
				CollisionParams.TraceTag = TraceTag;

			GetWorld()->SweepSingleByChannel(Hit, CurrentDestination,CurrentDestination + FVector::DownVector * MaxSweepDistance, FQuat::Identity, SweepChannel, CapsuleCollision, CollisionParams);

			if (Hit.bBlockingHit)
			{
				if (Hit.bStartPenetrating)
				{
					Data.SweepDelta = Hit.Normal * Hit.PenetrationDepth * 1.01f;
					CurrentDestination += Data.SweepDelta;

					GetWorld()->SweepSingleByChannel(Hit, CurrentDestination,CurrentDestination + FVector::DownVector * MaxSweepDistance, FQuat::Identity, SweepChannel, CapsuleCollision, CollisionParams);
				}

				if(Hit.bBlockingHit)
				{
					Data.SweepDelta = Hit.Location - CurrentDestination;
					CurrentDestination = Hit.Location;
				}
				else
				{
					UE_LOG(LogTeleportSystem, Warning, TEXT("Teleport system with name of: %s failed to teleport since the sweep didn't hit the floor"), *GetName());
					return false;
				}
			}
			else
			{
				UE_LOG(LogTeleportSystem, Warning, TEXT("Teleport system with name of: %s failed to teleport since the sweep didn't hit the floor"), *GetName());
				return false;
			}
		}

		LastPosition = TeleportTransform.InverseTransformPositionNoScale(Data.Player->GetActorLocation());
		
		if (LineTrace->CurrentlyHeldInteractable != nullptr)
		{
			FVector LocalHeldPosition = PlayerTransform.InverseTransformPosition(LineTrace->CurrentlyHeldInteractable->GetActorLocation());
			Data.Player->SetActorLocation(CurrentDestination);
			
			if(TeleportHeldItems)
				LineTrace->CurrentlyHeldInteractable->SetActorLocation(Data.Player->GetTransform().TransformPosition(LocalHeldPosition));
			else
				LineTrace->DropPickupable();
		}
		else
			Data.Player->SetActorLocation(CurrentDestination);

		OnTeleport();
		UE_LOG(LogTeleportSystem, Log, TEXT("Sweep delta for teleport: x: %f, y: %f, z: %f"), Data.SweepDelta.X, Data.SweepDelta.Y, Data.SweepDelta.Z);
		TimeOfTeleport = GetGameTimeSinceCreation();
		return true;
	}

	UE_LOG(LogTeleportSystem, Warning, TEXT("Player was null, teleport failed"));
	return false;
}
