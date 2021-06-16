#pragma once
#include "GameFramework/Actor.h"
#include "FGTeleportSystem.generated.h"

class AFGPlayer;
DECLARE_LOG_CATEGORY_EXTERN(LogTeleportSystem, Log, All);

struct FTeleportData
{
	FTeleportData(AFGPlayer* Player, FVector DestinationPosition, bool bDestinationIsRelative, FVector SweepDelta = FVector::ZeroVector)
	{
		this->Player = Player;
		this->DestinationPosition = DestinationPosition;
		this->bDestinationIsRelative = bDestinationIsRelative;
		this->SweepDelta = SweepDelta;
	}

	FTeleportData() {}
	
	AFGPlayer* Player;
	FVector DestinationPosition;
	bool bDestinationIsRelative;
	FVector SweepDelta;
};

class UBoxComponent;
UCLASS()
class AFGTeleportSystem : public AActor
{
	GENERATED_BODY()

public:
	AFGTeleportSystem();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TeleportBounds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	bool DontUpdateDestinationOnSweep = false;

	/** If the above teleport destination is relative to the player's current position */
	UPROPERTY(EditAnywhere)
	bool bRelativeDestination;

	/** If the teleport destination should be swept (so that the player never gets stuck in something and so that the player never falls to the floor after teleporting) */
	UPROPERTY(EditAnywhere)
	bool bSweepToFloor = false;

	/** The max distance the position will be swept, (if it does not find the floor the teleport will fail!) */
	UPROPERTY(EditAnywhere)
	float MaxSweepDistance = 500.f;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> SweepChannel;

	UPROPERTY(EditAnywhere)
	bool bDebugTrace = false;

	/** If true, the player will be teleported back to it's previous location for absolute locations and the reverse of it's offset for relative locations after the specified teleport back delay */
	UPROPERTY(EditAnywhere)
	bool bTeleportBack = false;

	UPROPERTY(EditAnywhere)
	float TeleportBackDelay = 0.f;

	/** The event pre teleport will be called first, then the system will wait this amount of seconds before actually teleporting */
	UPROPERTY(EditAnywhere)
	float DelayBeforeActualTeleport = 0.f;

	/** If pickupable items that are held should be teleported with the player */
	UPROPERTY(EditAnywhere)
	bool TeleportHeldItems = true;

	UFUNCTION(BlueprintImplementableEvent)
	void OnTeleport();

	UFUNCTION(BlueprintImplementableEvent)
	void PreTeleport();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTeleportBack();

	UPROPERTY(BlueprintReadOnly)
	FVector LastPosition;

private:
	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	bool Teleport(FTeleportData& Data);
	void QueueTeleport(const FTeleportData& Data);

	float TimeOfTeleport = -999.f;
	bool TeleportQueued = false;
	float TimeOfQueue = -999.f;
	FTeleportData SavedData;
	bool TeleportingBack = false;
};
