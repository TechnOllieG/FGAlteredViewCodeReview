#pragma once
#include "Blueprint/UserWidget.h"
#include "FGCrainMinimapWidget.generated.h"

class AFGCrainButtonLock;
class UCrainTargetComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogCrainMinimap, Log, All);

class ACrainActor;

UENUM(BlueprintType)
enum ECrainObjectTypes
{
	CrainTarget,
	CrainMagnet,
	CrainButton
};

UCLASS(BlueprintType, Blueprintable)
class UFGCrainMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(EditAnywhere)
    FVector2D CanvasResolution = FVector2D(1920.f, 1080.f);

	/** If this value is raised it will add more blank space on both sides of the widget */
	UPROPERTY(EditAnywhere)
	float CrainSpaceHorizontalMultiplier = 1.f;

	/** If this value is raised it will add more blank space on the top and bottom of the widget */
	UPROPERTY(EditAnywhere)
	float CrainSpaceVerticalMultiplier = 1.f;

	/** If true the aspect ratio of all objects will be forced to be 1x1 (will choose the smaller value of the bounds) */
	UPROPERTY(EditAnywhere)
	bool ForceAspectRatio1x1 = false;

	/** If true, the sizes of objects on the minimap will refer to the absolute sizes as specified below (force aspect ratio 1 by 1 will be overriden) */
	UPROPERTY(EditAnywhere)
	bool UseAbsoluteSizes = false;

	/** The absolute size of the magnet on the minimap (will only be used if use absolute sizes is true) */
	UPROPERTY(EditAnywhere)
	FVector2D MagnetAbsoluteSize;

	/** The absolute size of the targets on the minimap (will only be used if use absolute sizes is true) */
	UPROPERTY(EditAnywhere)
	FVector2D TargetsAbsoluteSize;

	/** The absolute size of the buttons on the minimap (will only be used if use absolute sizes is true) */
	UPROPERTY(EditAnywhere)
	FVector2D ButtonsAbsoluteSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACrainActor* CrainActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UCrainTargetComponent*> TargetComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AFGCrainButtonLock*> CrainButtonLocks;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
	FVector2D UsableCanvasResolution;

	UFUNCTION(BlueprintImplementableEvent)
	void SetUsableResolution(FVector2D UsableResolution);
	
	UFUNCTION(BlueprintImplementableEvent)
	void AddWidget(ECrainObjectTypes ObjectType, AActor* Object, const FVector2D& StartingSize, const FVector2D& StartingPosition, const float& StartRotInDegrees);

	UFUNCTION(BlueprintImplementableEvent)
	void SetWidgetTransform(AActor* Object, const FVector2D& Size, const FVector2D& Position, const float& RotationInDegrees);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetButtonState(AActor* ButtonObject, bool ButtonState);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMagnetState(bool State);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTargetPickupState(UCrainTargetComponent* TargetComponent, bool State);

private:
	FVector2D AreaMin2D;
	FVector2D AreaMax2D;
	FVector2D UsableCanvasMin;
	FVector2D UsableCanvasMax;

	void CalculatePositionSizeRotation(ECrainObjectTypes ObjectType, AActor* Actor, FTransform CrainTransform, FVector2D& Size, FVector2D& Position, float& Rotation);

	UFUNCTION()
	void HandleMagnetStatus(bool State);

	UFUNCTION()
	void HandleTargetPickupState(UCrainTargetComponent* TargetComponent, bool PickedUp);
};
