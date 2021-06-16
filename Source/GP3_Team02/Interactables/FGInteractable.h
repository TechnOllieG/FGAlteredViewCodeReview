#pragma once
#include "GameFramework/Actor.h"
#include "FGInteractable.generated.h"

class UFGTaskComponent;
class AFGInteractable;

DECLARE_LOG_CATEGORY_EXTERN(LogInteractionSystem, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractDelegate, AFGInteractable*, Interactable);

class UFGInteractableLineTraceComponent;
UCLASS()
class GP3_TEAM02_API AFGInteractable : public AActor
{
	GENERATED_BODY()

public:
	AFGInteractable();
	virtual void BeginPlay() override;
	virtual void Interact(UFGInteractableLineTraceComponent* LineTraceComponent);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UFGTaskComponent* TaskComponent;

	UPROPERTY(EditAnywhere)
	bool CompleteTaskComponentOnInteract = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CurrentlyInteractable = true;

	UPROPERTY(BlueprintAssignable)
	FInteractDelegate OnInteract;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Interact();
};