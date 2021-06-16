#pragma once
#include "Crain/CrainTargetComponent.h"
#include "GameFramework/GameState.h"
#include "Interactables/FGPickupInteractable.h"

#include "FGGameStateBase.generated.h"

class UFGCrainMinimapWidget;
class AFGCrainButtonLock;
class ACrainActor;
class AFGPlayer;
class AFGTaskTablet;

UCLASS()
class AFGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	AFGTaskTablet* GetTaskTablet();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFGCrainMinimapWidget* CrainMinimapWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UCrainTargetComponent*> CrainTargetComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AFGCrainButtonLock*> CrainButtonLocks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACrainActor* CrainActor;

private:
	UPROPERTY()
	AFGTaskTablet* TaskTablet;
};