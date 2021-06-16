#include "FGInteractable.h"

#include "GP3_Team02/Task Management/FGTaskComponent.h"

AFGInteractable::AFGInteractable()
{
	TaskComponent = CreateDefaultSubobject<UFGTaskComponent>(TEXT("Task Component"));
}

void AFGInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void AFGInteractable::Interact(UFGInteractableLineTraceComponent* LineTraceComponent)
{
	if(!CurrentlyInteractable)
		return;
	
	if(CompleteTaskComponentOnInteract)
		TaskComponent->CompleteTaskComponent();

	OnInteract.Broadcast(this);
	
	BP_Interact();
}
