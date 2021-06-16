#pragma once
#include "GameFramework/Actor.h"
#include "GP3_Team02/Task Management/FGTaskComponent.h"
#include "NumPad.generated.h"

class AFGCardScanner;
class ANumPadButton;
class FInteractDelegate;
class AFGInteractable;
class FInteractDelegate;

UENUM()
enum Colors
{
	Default,
	RedButton,
	GreenButton,
	BlueButton,
	YellowButton
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FANumPadCompleted, bool, Completed);

UCLASS()
class GP3_TEAM02_API ANumPad : public AActor
{
	GENERATED_BODY()
	
public:	
	ANumPad();	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	bool CheackCode();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_LockSolved();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UFGTaskComponent* TaskComponent;
	
	UPROPERTY(EditAnywhere)	
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;	
	
	UPROPERTY(EditAnywhere)
	UChildActorComponent* RedButton;
	UPROPERTY(EditAnywhere)
	UChildActorComponent* GreenButton;
	UPROPERTY(EditAnywhere)
	UChildActorComponent* BlueButton;
	UPROPERTY(EditAnywhere)
	UChildActorComponent* YellowButton;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<Colors>> CodeColors;

	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<Colors>> InputColors;

	UPROPERTY(VisibleAnywhere)
	int CurrentIndex = 0; 

	UFUNCTION()
	void RedButtonInteract(AFGInteractable* interact);
	UFUNCTION()
	void GreenButtonInteract(AFGInteractable* interact);
	UFUNCTION()
	void BlueButtonInteract(AFGInteractable* interact);
	UFUNCTION()
	void YellowButtonInteract(AFGInteractable* interact);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_InteractSound();

	FANumPadCompleted Completed;
};
