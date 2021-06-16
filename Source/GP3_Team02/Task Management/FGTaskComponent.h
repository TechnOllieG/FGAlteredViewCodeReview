#pragma once
#include "Components/ActorComponent.h"
#include "FGTaskComponent.generated.h"

class UFGTaskComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskComponentCompletedDelegate, UFGTaskComponent*, TaskComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskHandlerActivated);

UCLASS(meta=(BlueprintSpawnableComponent))
class UFGTaskComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFGTaskComponent();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CompleteTaskComponent();

	UFUNCTION()
	void OnActivateTaskComponent();

	UPROPERTY()
	FTaskComponentCompletedDelegate OnCompleteTaskComponent;

	UPROPERTY(BlueprintAssignable)
	FTaskHandlerActivated OnTaskHandlerActivated;
};
