#pragma once
#include "Blueprint/UserWidget.h"
#include "GP3_Team02/Task Management/FGTaskHandler.h"

#include "FGTaskTabletUIBase.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UFGTaskTabletUIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddTaskToTaskList(AFGTaskHandler* TaskHandler, bool CurrentlyActive);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTaskActive(AFGTaskHandler* TaskHandler, bool ActiveState);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetTaskComplete(AFGTaskHandler* TaskHandler, bool CompletionState);

	UFUNCTION(BlueprintCallable)
	TArray<AFGTaskHandler*> SortTaskHandlers(TArray<AFGTaskHandler*> TaskHandlers);
};