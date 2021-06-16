#include "FGTaskComponent.h"

#include "FGTaskHandler.h"

UFGTaskComponent::UFGTaskComponent()
{
	
}

void UFGTaskComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFGTaskComponent::CompleteTaskComponent()
{
	UE_LOG(LogTaskSystem, Log, TEXT("Task Component attached to actor with name %s was completed"), *GetOwner()->GetName());
	OnCompleteTaskComponent.Broadcast(this);
}

void UFGTaskComponent::OnActivateTaskComponent()
{
	OnTaskHandlerActivated.Broadcast();
}
