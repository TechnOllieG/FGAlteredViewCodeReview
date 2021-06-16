#include "FGTaskHandler.h"

#include "FGTaskComponent.h"
#include "Components/WidgetComponent.h"
#include "GP3_Team02/FGGameStateBase.h"
#include "GP3_Team02/UI/FGTaskTablet.h"
#include "GP3_Team02/UI/FGTaskTabletUIBase.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTaskSystem);

AFGTaskHandler::AFGTaskHandler()
{
	
}

void AFGTaskHandler::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<AFGGameStateBase>(UGameplayStatics::GetGameState(this));
	
	for(AActor* Actor : TaskComponentActors)
	{
		if(Actor == nullptr)
		{
			UE_LOG(LogTaskSystem, Error, TEXT("Task Handler with name %s has an actor in the array set to null"), *GetName());
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("Task Handler has an actor in the array set to null"));
			continue;
		}
		
		UFGTaskComponent* TaskComponent = Actor->FindComponentByClass<UFGTaskComponent>();
		if(TaskComponent == nullptr)
		{
			UE_LOG(LogTaskSystem, Error, TEXT("Task Handler with name %s has one or more actors in it's task component actors array that does not have a task component"), *GetName());
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, TEXT("Task Handler has one or more actors in it's task component actors array that does not have a task component"));
			return;
		}
		BP_OnActivateTask.AddDynamic(TaskComponent, &UFGTaskComponent::OnActivateTaskComponent);
		TaskComponents.Add(TaskComponent);
		TaskComponent->OnCompleteTaskComponent.AddDynamic(this, &AFGTaskHandler::OnCompleteTaskComponent);
	}

	AFGTaskTablet* Tablet = GameState->GetTaskTablet();
	
	if(Tablet != nullptr)
	{
		Tablet->WidgetComponent->InitWidget();
		UUserWidget* UserWidget = Tablet->WidgetComponent->GetWidget();
		
		Widget = Cast<UFGTaskTabletUIBase>(UserWidget);
		Widget->AddTaskToTaskList(this, ActiveAtStart);
	}
}

void AFGTaskHandler::OnCompleteTaskComponent(UFGTaskComponent* TaskComponent)
{
	TaskComponent->OnCompleteTaskComponent.RemoveAll(this);
	
	CurrentCompletedComponents++;

	if(CurrentCompletedComponents >= TaskComponents.Num())
	{
		BP_OnCompleteTask();
		OnCompleteTaskDelegate.Broadcast();

		if(Widget == nullptr)
			return;
		
		Widget->SetTaskComplete(this, true);
		
		if(ShouldActivateOtherTaskWhenComplete)
			OnActivateTask();
	}
}

void AFGTaskHandler::OnActivateTask()
{
	Widget->SetTaskActive(TaskToActivate, true);
	TaskToActivate->BP_OnActivateTask.Broadcast();
}

