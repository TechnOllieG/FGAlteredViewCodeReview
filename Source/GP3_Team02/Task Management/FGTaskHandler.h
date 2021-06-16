#pragma once
#include "GameFramework/Actor.h"
#include "FGTaskHandler.generated.h"

class UFGTaskTabletUIBase;
DECLARE_LOG_CATEGORY_EXTERN(LogTaskSystem, Log, All);

class AFGGameStateBase;
class UFGTaskComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActivateTask);

UCLASS()
class AFGTaskHandler : public AActor
{
	GENERATED_BODY()

public:
	AFGTaskHandler();
	virtual void BeginPlay() override;

	/** Higher number means this task will end up higher up in the task list */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	int TaskPriority = 0;

	/** The description/name of the task that will show up in the task list */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FString TaskDisplayDescription = "Unnamed task";

	/** If this task should be active on the task list at the start of the game */
	UPROPERTY(EditInstanceOnly)
	bool ActiveAtStart = true;

	/** If the completion of this task should activate another task on the task list */
	UPROPERTY(EditInstanceOnly)
	bool ShouldActivateOtherTaskWhenComplete = false;

	/** If above bool is true, the specified task will get activated on the task list when this task is completed */
	UPROPERTY(EditInstanceOnly)
	AFGTaskHandler* TaskToActivate;

	/** Actors with task components that should be bound to this specific task */
	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> TaskComponentActors;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnCompleteTask();

	UPROPERTY(BlueprintAssignable)
	FOnActivateTask OnCompleteTaskDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnActivateTask BP_OnActivateTask;

private:
	UFUNCTION()
	void OnCompleteTaskComponent(UFGTaskComponent* TaskComponent);

	void OnActivateTask();
	
	int CurrentCompletedComponents = 0;
	AFGGameStateBase* GameState;
	UFGTaskTabletUIBase* Widget;
	TArray<UFGTaskComponent*> TaskComponents;
};