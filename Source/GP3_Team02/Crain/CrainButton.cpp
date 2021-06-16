#include "CrainButton.h"

#include "CrainActor.h"
#include "GP3_Team02/Character/FGInteractableLineTraceComponent.h"

ACrainButton::ACrainButton()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
}

void ACrainButton::BeginPlay()
{
	Super::BeginPlay();
}

void ACrainButton::Interact(UFGInteractableLineTraceComponent* LineTraceComponent)
{
	Super::Interact(LineTraceComponent);

	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	
	FVector MoveDir;

	switch (MovementDirection)
	{
		case Left:
			MoveDir = -CrainActor->GetActorRightVector();
			break;
		
		case Right:
			MoveDir = CrainActor->GetActorRightVector();
			break;
			
		case Up:
			MoveDir = CrainActor->GetActorUpVector();
			break;
			
		case Down:
			MoveDir = -CrainActor->GetActorUpVector();
			break;
		
		case Forwards:
			MoveDir = CrainActor->GetActorForwardVector();
			break;
		
		case Backwards:
			MoveDir = -CrainActor->GetActorForwardVector();
			break;

		case Stop:
			MoveDir = FVector::ZeroVector;
			break;
		
		case TurnOffOnMagnet:
			CrainActor->bMagnetIsOn = !CrainActor->bMagnetIsOn;
			LineTraceComponent->bIsInteracting = !LineTraceComponent->bIsInteracting;
		
		default:
			MoveDir = FVector::ZeroVector;
			break;
	}
	
	CrainActor->Move(MoveDir);
}