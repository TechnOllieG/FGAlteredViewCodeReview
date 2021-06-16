#include "FGGlitchSystem.h"

DEFINE_LOG_CATEGORY(LogGlitchSystem);

AFGGlitchSystem::AFGGlitchSystem()
{
	
}

void AFGGlitchSystem::GlitchStart()
{
	BP_OnGlitchStart();
	
	for(TPair<AActor*, FActorGlitchValues>& GlitchModule : GlitchSettings)
	{
		AActor* Actor = GlitchModule.Key;
		
		ActorOriginalPositions.Add(Actor->GetActorLocation());
		ActorOriginalRotations.Add(Actor->GetActorRotation());

		if(GlitchModule.Value.bGlitchPosition)
			Actor->AddActorWorldOffset(GlitchModule.Value.RelativePosition, GlitchModule.Value.bEnableSweepOnChangePosition, nullptr, ETeleportType::ResetPhysics);

		if(GlitchModule.Value.bGlitchRotation)
			Actor->SetActorRotation(GlitchModule.Value.Rotation, ETeleportType::ResetPhysics);
		
		UStaticMeshComponent* MeshComponent = Actor->FindComponentByClass<UStaticMeshComponent>();
		if(MeshComponent != nullptr)
		{
			ActorOriginalMeshComponents.Add(MeshComponent);
			ActorOriginalMaterials.Add(MeshComponent->GetMaterial(0));

			if(GlitchModule.Value.bGlitchMaterial)
			{
				if(GlitchModule.Value.Material != nullptr)
					MeshComponent->SetMaterial(0, GlitchModule.Value.Material);
				else
					UE_LOG(LogGlitchSystem, Warning, TEXT("Can't change material of actor since material property is null"));
			}

			if(GlitchModule.Value.bDisablePhysicsOnGlitch)
			{
				if(MeshComponent->IsSimulatingPhysics())
					MeshComponent->SetSimulatePhysics(false);
				else
				{
					UE_LOG(LogGlitchSystem, Warning, TEXT("Tried to disable physics on actor with name: %s when physics is already disabled"), *Actor->GetName());
					GlitchModule.Value.bDisablePhysicsOnGlitch = false;
				}
			}
		}
		else
		{
			UE_LOG(LogGlitchSystem, Warning, TEXT("Found actor with name: %s without static mesh component, can't change materials"), *Actor->GetName());
			ActorOriginalMeshComponents.Add(nullptr);
			ActorOriginalMaterials.Add(nullptr);
		}
	}
}

void AFGGlitchSystem::GlitchEnd()
{	
	if(ActorOriginalPositions.Num() == 0)
	{
		UE_LOG(LogGlitchSystem, Warning, TEXT("Trying to end glitch before glitch has started"));
		return;
	}

	BP_OnGlitchEnd();

	int Iterator = 0;
	for(const TPair<AActor*, FActorGlitchValues>& GlitchModule : GlitchSettings)
	{
		AActor* Actor = GlitchModule.Key;

		Actor->SetActorLocation(ActorOriginalPositions[Iterator], false, nullptr, ETeleportType::ResetPhysics);
		Actor->SetActorRotation(ActorOriginalRotations[Iterator], ETeleportType::ResetPhysics);

		UStaticMeshComponent* MeshComponent = ActorOriginalMeshComponents[Iterator];
		
		if(MeshComponent != nullptr)
		{
			if(GlitchModule.Value.bGlitchMaterial)
				MeshComponent->SetMaterial(0, ActorOriginalMaterials[Iterator]);

			if(GlitchModule.Value.bDisablePhysicsOnGlitch)
				MeshComponent->SetSimulatePhysics(true);
		}

		Iterator++;
	}

	ActorOriginalMaterials.Empty();
	ActorOriginalPositions.Empty();
	ActorOriginalRotations.Empty();
	ActorOriginalMeshComponents.Empty();
}