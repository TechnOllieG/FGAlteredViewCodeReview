#include "FGGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "UI/FGTaskTablet.h"

void AFGGameStateBase::BeginPlay()
{
	Super::BeginPlay();
	GetTaskTablet();
}

AFGTaskTablet* AFGGameStateBase::GetTaskTablet()
{
	if(TaskTablet == nullptr)
	{
		AActor* Actor = UGameplayStatics::GetActorOfClass(this, AFGTaskTablet::StaticClass());
	
		if(Actor != nullptr)
			TaskTablet = Cast<AFGTaskTablet>(Actor);
		else
			UE_LOG(LogTemp, Warning, TEXT("Couldn't find task tablet in level, is this intentional?"));
	}
	return TaskTablet;
}