#pragma once

#include "Components/ActorComponent.h"
#include "CrainTargetComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class UCrainTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCrainTargetComponent();
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
};