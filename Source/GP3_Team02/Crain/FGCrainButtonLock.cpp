#include "FGCrainButtonLock.h"

#include "GP3_Team02/FGGameStateBase.h"
#include "Kismet/GameplayStatics.h"

void AFGCrainButtonLock::BeginPlay()
{
	Super::BeginPlay();
	AFGGameStateBase* GameState = Cast<AFGGameStateBase>(UGameplayStatics::GetGameState(this));
	if(GameState != nullptr)
		GameState->CrainButtonLocks.Add(this);
	else
		UE_LOG(LogTemp, Warning, TEXT("Crain Button Lock couldn't get GameState"));
}
