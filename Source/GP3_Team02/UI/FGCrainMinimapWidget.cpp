#include "FGCrainMinimapWidget.h"

#include "GP3_Team02/FGGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "GP3_Team02/Crain/CrainActor.h"
#include "Components/BillboardComponent.h"
#include "GP3_Team02/Crain/FGCrainButtonLock.h"
#include "GP3_Team02/Library/FGMathLibrary.h"

DEFINE_LOG_CATEGORY(LogCrainMinimap);

void UFGCrainMinimapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AFGGameStateBase* GameState = Cast<AFGGameStateBase>(UGameplayStatics::GetGameState(this));
	if (GameState != nullptr)
	{
		GameState->CrainMinimapWidget = this;

		CrainActor = GameState->CrainActor;
		TargetComponents = GameState->CrainTargetComponents;
		CrainButtonLocks = GameState->CrainButtonLocks;

		const FTransform CrainTransform = CrainActor->GetTransform();

		const FVector Right = CrainActor->RightBillboardComponent->GetRelativeLocation();
		const FVector Left = CrainActor->LeftBillboardComponent->GetRelativeLocation();

		const FVector Forward = CrainActor->ForwardBillboardComponent->GetRelativeLocation();
		const FVector Backward = CrainActor->BackwardBillboardComponent->GetRelativeLocation();

		const FVector Up = CrainActor->UpBillboardComponent->GetRelativeLocation();
		const FVector Down = CrainActor->DownBillboardComponent->GetRelativeLocation();

		const FVector AreaMin = Left + Backward + Down;
		const FVector AreaMax = Right + Forward + Up;

		AreaMin2D = FVector2D(AreaMin.Y * CrainSpaceHorizontalMultiplier, -AreaMax.X * CrainSpaceVerticalMultiplier);
		AreaMax2D = FVector2D(AreaMax.Y * CrainSpaceHorizontalMultiplier, -AreaMin.X * CrainSpaceVerticalMultiplier);

		const FVector2D AreaDimensions = FVector2D(AreaMax2D.X - AreaMin2D.X, AreaMax2D.Y - AreaMin2D.Y);

		SetMinimumDesiredSize(CanvasResolution);

		// Calculate aspect ratio where y is 1, example: 2.35:1
		const float AreaAspectRatioX = AreaDimensions.X / AreaDimensions.Y;
		const float CanvasAspectRatioX = CanvasResolution.X / CanvasResolution.Y;

		UE_LOG(LogCrainMinimap, Log, TEXT("Aspect ratio X for area: %f, Aspect ratio X for canvas: %f"),
		       AreaAspectRatioX, CanvasAspectRatioX);

		UsableCanvasResolution = CanvasResolution;

		if (AreaAspectRatioX > CanvasAspectRatioX) // Letterboxes (decrease y resolution)
		{
			UsableCanvasResolution.Y = UsableCanvasResolution.X / AreaAspectRatioX;
		}
		else // Pillarboxes (decrease x resolution)
		{
			UsableCanvasResolution.X = UsableCanvasResolution.Y * AreaAspectRatioX;
		}

		UsableCanvasMax = UsableCanvasResolution * 0.5f;
		UsableCanvasMin = -UsableCanvasMax;

		SetUsableResolution(UsableCanvasResolution);

		FVector2D Size, Position;
		float Rotation;

		for (AFGCrainButtonLock* ButtonLock : CrainButtonLocks)
		{
			CalculatePositionSizeRotation(ECrainObjectTypes::CrainButton, ButtonLock, CrainTransform, Size, Position,
			                              Rotation);
			AddWidget(ECrainObjectTypes::CrainButton, ButtonLock, Size, Position, Rotation);
			SetButtonState(ButtonLock, false);
		}

		for (UCrainTargetComponent* TargetComponent : TargetComponents)
		{
			AActor* Actor = TargetComponent->GetOwner();

			CalculatePositionSizeRotation(ECrainObjectTypes::CrainTarget, Actor, CrainTransform, Size, Position,
			                              Rotation);
			AddWidget(ECrainObjectTypes::CrainTarget, Actor, Size, Position, Rotation);
			SetTargetPickupState(TargetComponent, false);
		}

		CalculatePositionSizeRotation(ECrainObjectTypes::CrainMagnet, CrainActor, CrainTransform, Size, Position,
		                              Rotation);
		AddWidget(ECrainObjectTypes::CrainMagnet, CrainActor, Size, Position, Rotation);
		SetMagnetState(false);

		CrainActor->MagnetDelegate.AddDynamic(this, &UFGCrainMinimapWidget::HandleMagnetStatus);
		CrainActor->CrainDelegate.AddDynamic(this, &UFGCrainMinimapWidget::HandleTargetPickupState);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("FGCrainMinimapWidget couldn't get GameState"));
}

void UFGCrainMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FTransform CrainTransform = CrainActor->GetTransform();

	FVector2D Size, Position;
	float Rotation;

	for (AFGCrainButtonLock* ButtonLock : CrainButtonLocks)
	{
		CalculatePositionSizeRotation(ECrainObjectTypes::CrainButton, ButtonLock, CrainTransform, Size, Position,
		                              Rotation);
		SetWidgetTransform(ButtonLock, Size, Position, Rotation);
	}

	for (UCrainTargetComponent* TargetComponent : TargetComponents)
	{
		AActor* Actor = TargetComponent->GetOwner();

		CalculatePositionSizeRotation(ECrainObjectTypes::CrainTarget, Actor, CrainTransform, Size, Position, Rotation);
		SetWidgetTransform(Actor, Size, Position, Rotation);
	}

	CalculatePositionSizeRotation(ECrainObjectTypes::CrainMagnet, CrainActor, CrainTransform, Size, Position, Rotation);
	SetWidgetTransform(CrainActor, Size, Position, Rotation);
}

void UFGCrainMinimapWidget::CalculatePositionSizeRotation(ECrainObjectTypes ObjectType, AActor* Actor,
                                                          FTransform CrainTransform, FVector2D& Size,
                                                          FVector2D& Position, float& Rotation)
{
	UStaticMeshComponent* StaticMeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent == nullptr)
	{
		UE_LOG(LogCrainMinimap, Warning,
		       TEXT("Skipped adding crain target since the actor didn't contain a static mesh component"));
		return;
	}

	FVector LocalPosition = CrainTransform.InverseTransformPosition(StaticMeshComponent->GetComponentLocation());
	FVector2D LocalPosition2D = FVector2D(LocalPosition.Y, -LocalPosition.X);

	if (!UseAbsoluteSizes)
	{
		const FVector Extent = StaticMeshComponent->Bounds.BoxExtent;
		Size = FGMathLibrary::Remap(AreaMin2D, AreaMax2D, UsableCanvasMin, UsableCanvasMax,
		                            FVector2D(Extent.Y, Extent.X) * 2.f);

		if (ForceAspectRatio1x1)
		{
			if (Size.X < Size.Y)
				Size.X = Size.Y;
			else
				Size.Y = Size.X;
		}
	}
	else
	{
		switch (ObjectType)
		{
		case CrainTarget:
			Size = TargetsAbsoluteSize;
			break;

		case CrainMagnet:
			Size = MagnetAbsoluteSize;
			break;

		case CrainButton:
			Size = ButtonsAbsoluteSize;
			break;

		default: break;
		}


		Position = FGMathLibrary::Remap(AreaMin2D, AreaMax2D, UsableCanvasMin, UsableCanvasMax, LocalPosition2D);
		Rotation = StaticMeshComponent->GetComponentRotation().Yaw;
	}
}

void UFGCrainMinimapWidget::HandleMagnetStatus(bool State)
{
	SetMagnetState(State);
}

void UFGCrainMinimapWidget::HandleTargetPickupState(UCrainTargetComponent* TargetComponent, bool PickedUp)
{
	SetTargetPickupState(TargetComponent, PickedUp);
}
