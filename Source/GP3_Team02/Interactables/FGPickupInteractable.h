#pragma once
#include "FGInteractable.h"
#include "FGPickupInteractable.generated.h"

class UFGInteractableLineTraceComponent;

UCLASS()
class GP3_TEAM02_API AFGPickupInteractable : public AFGInteractable
{
	GENERATED_BODY()

public:
	AFGPickupInteractable();
	virtual void BeginPlay() override;
	virtual void Interact(UFGInteractableLineTraceComponent* LineTraceComponent) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector PickupPivotOffset;

	/** Used to determine the speed of the object when dropped by the player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Mass = 100.f;

	UPROPERTY(EditDefaultsOnly)
	bool RotateAroundAxisWhenHeld = false;

	UPROPERTY(EditDefaultsOnly)
	FVector Axis;

	UPROPERTY(EditDefaultsOnly)
	float AxisRotationDegrees;

	FVector Velocity;
	FVector RotationVelocity;

	FVector StartPosition;
	FRotator StartRotation;
	
	void OnPickup(UFGInteractableLineTraceComponent* LineTraceComponent);
	UFUNCTION(BlueprintCallable)
	void DropItem();
	
	void OnDrop();

	void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	bool bIsPickedUp = false;

	FVector HeldVelocity;

	UFGInteractableLineTraceComponent* LineTraceComponentPlayer;
};


