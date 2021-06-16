#include "CrainActor.h"
#include "DrawDebugHelpers.h"
#include "Components/BillboardComponent.h"
#include "GP3_Team02/FGGameStateBase.h"
#include "GP3_Team02/Interactables/FGPickupInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "CrainTargetComponent.h"

ACrainActor::ACrainActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneComponent);

	MagnetPoint = CreateDefaultSubobject<USceneComponent>(TEXT("MagnetPoint"));
	MagnetPoint->SetupAttachment(StaticMesh);
	
	ForwardBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("ForwardBillboardComponent"));
	ForwardBillboardComponent->SetupAttachment(SceneComponent);
	BackwardBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BackwardBillboardComponent"));
	BackwardBillboardComponent->SetupAttachment(SceneComponent);
	LeftBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("LeftBillboardComponent"));
	LeftBillboardComponent->SetupAttachment(SceneComponent);
	RightBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("RightBillboardComponent"));
	RightBillboardComponent->SetupAttachment(SceneComponent);
	UpBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("UpBillboardComponent"));
	UpBillboardComponent->SetupAttachment(SceneComponent);
	DownBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("DownBillboardComponent"));
	DownBillboardComponent->SetupAttachment(SceneComponent);
	
	ForwardBillboardComponent->		SetRelativeLocation(GetActorForwardVector()  *	200);
	BackwardBillboardComponent->	SetRelativeLocation(-GetActorForwardVector() *	200);
	RightBillboardComponent->		SetRelativeLocation(GetActorRightVector()	 *	200);
	LeftBillboardComponent->		SetRelativeLocation(-GetActorRightVector()	 *	200);
	UpBillboardComponent->			SetRelativeLocation(GetActorUpVector()		 *	200);
	DownBillboardComponent->		SetRelativeLocation(-GetActorUpVector()		 *	200);
}

void ACrainActor::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AFGGameStateBase>(UGameplayStatics::GetGameState(this));
	Location = StaticMesh->GetRelativeLocation();
	GameMode->CrainActor = this;

}

void ACrainActor::Move(FVector MoveDir)
{
	MoveDirection = MoveDir;
}

FVector ACrainActor::ClampVector(FVector pos, FVector min, FVector max)
{
	if (pos.X > max.X)
	{
		pos.X = max.X;
	}
	else if(pos.X < min.X)
	{
		pos.X = min.X;
	}

	if (pos.Y > max.Y)
	{
		pos.Y = max.Y;
	}
	else if(pos.Y < min.Y)
	{
		pos.Y = min.Y;
	}

	if (pos.Z > max.Z)
	{
		pos.Z = max.Z;
	}
	else if(pos.Z < min.Z)
	{
		pos.Z = min.Z;
	}
	
	return pos;
}

void ACrainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Cheak if the frain os on.
	if (CrainOn == false)
		return;

	// Magnet movement and clamp
	FVector Right = RightBillboardComponent->GetRelativeLocation();
	FVector Forward = ForwardBillboardComponent->GetRelativeLocation();

	FVector Left = LeftBillboardComponent->GetRelativeLocation();
	FVector Backward = BackwardBillboardComponent->GetRelativeLocation();

	FVector Up = UpBillboardComponent->GetRelativeLocation();
	FVector Down = DownBillboardComponent->GetRelativeLocation();
	
	FVector Max = Right + Forward + Up;
	FVector Min = Left + Backward + Down;
	
	Location += Speed * DeltaTime * MoveDirection;

	Location = ClampVector(Location, Min, Max);
	
	MoveDirection = FVector::ZeroVector;
	
	StaticMesh->SetRelativeLocation(Location);
	//End OF Magnet movement
	
	if (bMagnetIsOn == false)
	{
		if (MagnetState == true)
		{
			MagnetState = false;
			MagnetDelegate.Broadcast(MagnetState);
		}

		for (int i = 0; i < TargetComponents.Num(); i++)
		{			
			if (TargetStates[i] == true)
			{
				UCrainTargetComponent* TargetComp = TargetComponents[i];
				TargetStates[i] = false;
				CrainDelegate.Broadcast(TargetComp, TargetStates[i]);
			}				
		}
		
		return;
	}
	else
	{
		if (MagnetState == false)
		{
			MagnetState = true;
			MagnetDelegate.Broadcast(MagnetState);
		} 

		TargetComponents = GameMode->CrainTargetComponents;
		TargetStates.SetNum(TargetComponents.Num(), false);
		
		FVector MyLocation = MagnetPoint->GetComponentLocation();

		DrawDebugSphere(GetWorld(), MyLocation,MagnetRange, 16, FColor::Red, false, 0 , 6);

		//Go Through all the Crain Target Components in the level.
		
		for (int i = 0; i < TargetComponents.Num(); i++)
		{
			UCrainTargetComponent* TargetComp = TargetComponents[i];

			if (TargetComp != nullptr) 
			{			
				AActor* Actor = TargetComp->GetOwner();
			
				AFGPickupInteractable* PickUp = Cast<AFGPickupInteractable>(Actor);
				if (PickUp != nullptr && PickUp->bIsPickedUp == true) continue;
				
				UPrimitiveComponent* Primitive = Actor->FindComponentByClass<UPrimitiveComponent>();
			
				float DistanceToTargets = FVector::Distance(MyLocation, Primitive->GetComponentLocation());

				//Cheak if distance is inside the magnets range
				if (DistanceToTargets < MagnetRange)
				{
					if (TargetStates[i] == false)
					{
						TargetStates[i] = true;
						CrainDelegate.Broadcast(TargetComp, TargetStates[i]);
					}					
					
					if (Primitive == nullptr)
					{
						UE_LOG(LogTemp,Warning, TEXT("Mesh Was nullptr"));
						return;
					}						

					//Add force towards the the desired position
					Primitive->AddForceAtLocation((MyLocation - Actor->GetActorLocation()) * MagnetStrength * Primitive->GetMass(), Actor->GetActorLocation(), NAME_None);
				}
				else if (TargetStates[i] == true)
				{
					TargetStates[i] = false;
					CrainDelegate.Broadcast(TargetComp, TargetStates[i]);
				}				
			}
		}
	}
}