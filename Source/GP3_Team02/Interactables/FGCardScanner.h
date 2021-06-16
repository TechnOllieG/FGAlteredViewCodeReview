#pragma once
#include "FGInteractable.h"
#include "FGCardScanner.generated.h"

class UFGTaskComponent;
class AFGCard;
class UBoxComponent;
class USceneComponent;
class UMeshComponent;

UCLASS()
class GP3_TEAM02_API AFGCardScanner : public AFGInteractable
{
	GENERATED_BODY()
	public:
	AFGCardScanner();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UBoxComponent* BoxComponent;
	
	virtual void Interact(UFGInteractableLineTraceComponent* LineTraceComponent) override;
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	FName PassWord = "Sus";
	
	UPROPERTY(EditAnywhere)
	bool CompleteTaskComponentOnUnlock = false;

	UPROPERTY(EditAnywhere)
	bool CompleteTaskComponentOnOpen = false;

	UFUNCTION(BlueprintCallable)
	void ScanKeyCard(AFGCard* Card);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ScanKeyCard();
		
	virtual void OpenDoor();
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void BP_OpenDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Unlocked = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOpen = false;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit);
		
};
