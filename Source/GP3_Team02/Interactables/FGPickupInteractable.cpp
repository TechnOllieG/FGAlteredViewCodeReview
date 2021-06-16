#include "FGPickupInteractable.h"

#include "DrawDebugHelpers.h"
#include "GP3_Team02/FGGameStateBase.h"
#include "GP3_Team02/Character/FGInteractableLineTraceComponent.h"
//#include "GP3_Team02/Character/FGPlayer.h"
#include "Kismet/GameplayStatics.h"

AFGPickupInteractable::AFGPickupInteractable()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);
	Mesh->SetGenerateOverlapEvents(true);
}

void AFGPickupInteractable::BeginPlay()
{
	Super::BeginPlay();	 
	AFGGameStateBase* GameMode = Cast<AFGGameStateBase>(UGameplayStatics::GetGameState(this));

	StartPosition = GetActorLocation();
	StartRotation = GetActorRotation();
}

void AFGPickupInteractable::Interact(UFGInteractableLineTraceComponent* LineTraceComponent)
{
	Super::Interact(LineTraceComponent);
	OnPickup(LineTraceComponent);
}

void AFGPickupInteractable::OnPickup(UFGInteractableLineTraceComponent* LineTraceComponent)
{
	LineTraceComponentPlayer = LineTraceComponent;
	bIsPickedUp = true;
	Mesh->SetSimulatePhysics(false);
	LineTraceComponent->PickupInteractable(this);
}

void AFGPickupInteractable::DropItem()
{
	Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
	Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	
	if (LineTraceComponentPlayer != nullptr)	
		LineTraceComponentPlayer->DropPickupable();
}

void AFGPickupInteractable::OnDrop()
{	
	LineTraceComponentPlayer = nullptr;
	bIsPickedUp = false;
	Mesh->SetSimulatePhysics(true);
	Mesh->SetPhysicsLinearVelocity(HeldVelocity / Mass);
	Mesh->SetPhysicsAngularVelocityInDegrees(Mesh->GetPhysicsAngularVelocityInDegrees() / Mass);
}

void AFGPickupInteractable::Tick(float DeltaSeconds)
{	
	Super::Tick(DeltaSeconds);	
}