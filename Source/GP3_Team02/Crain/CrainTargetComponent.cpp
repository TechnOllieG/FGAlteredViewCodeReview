#include "CrainTargetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GP3_Team02/FGGameStateBase.h"


UCrainTargetComponent::UCrainTargetComponent()
{
	
}

void UCrainTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Actor = GetOwner();
	UPrimitiveComponent* Owner = Actor->FindComponentByClass<UPrimitiveComponent>();
	Owner->SetSimulatePhysics(true);
	AFGGameStateBase* GameMode = Cast<AFGGameStateBase>(UGameplayStatics::GetGameState(this));
	GameMode->CrainTargetComponents.Add(this);	
}

void UCrainTargetComponent::InitializeComponent()
{
	Super::InitializeComponent();

}
