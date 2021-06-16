#pragma once
#include "GameFramework/Actor.h"
#include "CrainActor.generated.h"

class AFGGameStateBase;
class UCrainTargetComponent;
class UBillboardComponent;
class ACrainActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrainMagnetDelegate, bool, MagnetIsOn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCrainDelegate, UCrainTargetComponent*, CrainTargetComponent, bool, TargetPickedUp );


UCLASS()
class GP3_TEAM02_API ACrainActor : public AActor
{
	GENERATED_BODY()
public:
	ACrainActor();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneComponent;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* MagnetPoint;

	//Billboards are used to set the bounding box of the crande movement.
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* ForwardBillboardComponent;
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* BackwardBillboardComponent;
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* LeftBillboardComponent;
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* RightBillboardComponent;
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* UpBillboardComponent;
	UPROPERTY(EditDefaultsOnly)
	UBillboardComponent* DownBillboardComponent;

	FCrainDelegate CrainDelegate;
	TArray<bool> TargetStates;
	
	FCrainMagnetDelegate MagnetDelegate;
	bool MagnetState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CrainOn = false;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel;
	
	//Magnet
	UPROPERTY(EditAnywhere)
	float MagnetRange = 300;
	UPROPERTY(EditAnywhere)
	float MagnetStrength = 100;
	UPROPERTY(EditAnywhere)
	bool bMagnetIsOn = false;

	UFUNCTION(BlueprintCallable)
	void Move(FVector MoveDir);

	FVector ClampVector(FVector pos, FVector min , FVector max);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<UCrainTargetComponent*> TargetComponents;

	AFGGameStateBase* GameMode;

	FVector MoveDirection;

	UPROPERTY(VisibleAnywhere)
	FVector Location;
		
	UPROPERTY(EditAnywhere)
	float Speed = 100;
};
