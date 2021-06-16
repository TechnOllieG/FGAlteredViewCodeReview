#pragma once
#include "GameFramework/Actor.h"
#include "FGGlitchSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGlitchSystem, Log, All);

USTRUCT()
struct FActorGlitchValues
{
	GENERATED_BODY()

	/** If this actor should change position when glitch starts */
	UPROPERTY(EditInstanceOnly)
	bool bGlitchPosition;

	/** The relative position (world offset) this actor should have applied when glitch starts */
	UPROPERTY(EditInstanceOnly)
	FVector RelativePosition;

	/** If this actor should change rotation when glitch starts */
	UPROPERTY(EditInstanceOnly)
	bool bGlitchRotation;

	/** The absolute rotation this actor should be set to when glitch starts */
	UPROPERTY(EditInstanceOnly)
	FRotator Rotation;

	/** If this actor should have it's material (on the first found static mesh component) changed when glitch starts */
	UPROPERTY(EditInstanceOnly)
	bool bGlitchMaterial;
	
	/** The material that should be changed on the actors static mesh component (the first one that is found) when glitch starts */
	UPROPERTY(EditInstanceOnly)
	UMaterialInterface* Material;

	/** If physics should be disabled for the actors static mesh component when glitching (and reverted when the glitch stops) */
	UPROPERTY(EditInstanceOnly)
	bool bDisablePhysicsOnGlitch;

	/** Enable this bool if position changes should sweep so that the actor can't pass through any other objects while moving, note that sweep is not supported with rotation, so add a positional offset if a rotation change results in collisions */
	UPROPERTY(EditInstanceOnly)
	bool bEnableSweepOnChangePosition;
};

UCLASS()
class AFGGlitchSystem : public AActor
{
	GENERATED_BODY()

public:
	AFGGlitchSystem();

	/** The actors that should be modified when the glitch starts and reverted when glitch ends and their settings during the glitch */
	UPROPERTY(EditInstanceOnly)
	TMap<AActor*, FActorGlitchValues> GlitchSettings;
	
	UFUNCTION(BlueprintCallable)
	void GlitchStart();

	UFUNCTION(BlueprintCallable)
	void GlitchEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGlitchStart();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGlitchEnd();

private:
	TArray<FVector> ActorOriginalPositions;

	TArray<FRotator> ActorOriginalRotations;

	UPROPERTY()
	TArray<UStaticMeshComponent*> ActorOriginalMeshComponents;
	
	UPROPERTY()
	TArray<UMaterialInterface*> ActorOriginalMaterials;
};