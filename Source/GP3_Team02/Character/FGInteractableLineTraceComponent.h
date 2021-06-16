#pragma once
#include "Components/ActorComponent.h"
#include "FGInteractableLineTraceComponent.generated.h"

class AFGInteractable;
class AFGPickupInteractable;
class UCameraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupInteractableDelegate, AFGPickupInteractable*, CurrentlyHeldInteractable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInteractableFeedbackDelegate, AFGInteractable*, RelevantInteractable, bool, FeedbackState);

UCLASS()
class GP3_TEAM02_API UFGInteractableLineTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFGInteractableLineTraceComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	float RaycastRange = 1000.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(EditAnywhere)
	bool DebugTrace = false;
	
	UPROPERTY(EditAnywhere)
	FLinearColor TraceDebugColor;

	/** How far (in units) in front of the owner actor any held items should be kept at */
	UPROPERTY(EditAnywhere)
	float HeldInteractableDistance = 200.f;

	UPROPERTY(EditAnywhere)
	float MaxHeightOffset = 200.f;

	UPROPERTY(EditAnywhere)
	float CollisionForceMultiplier = 10.f;

	UPROPERTY(BlueprintReadOnly)
	AFGPickupInteractable* CurrentlyHeldInteractable = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AFGInteractable* OutlinedInteractable = nullptr;

	UPROPERTY(BlueprintAssignable)
	FPickupInteractableDelegate BP_OnPickupInteractable;

	UPROPERTY(BlueprintAssignable)
	FInteractableFeedbackDelegate BP_OnInteractableFeedback;

	UFUNCTION(BlueprintCallable)
	void DropPickupable();

	void PickupInteractable(AFGPickupInteractable* Pickup);

	void StartInteract();
	void EndInteract();
	void HandleInteract();
	void HoldHandleInteract();
	void TurnOnInteractableFeedback(AFGInteractable* Interactable);
	void TurnOffInteractableFeedback();

	bool bIsInteracting = false;

	/** if an interactable is above this distance from the player it will be dropped */
	UPROPERTY(EditAnywhere)
	float MaxDistance = 10;

	UPROPERTY(EditAnywhere)
	int MovementIterativeDepth = 3;

private:
	UCameraComponent* PlayerCamera;
	TArray<AActor*> AttachedActors;
	static bool SortFHitResult(FHitResult A, FHitResult B);
};