#include "FGCardScanner.h"

#include "FGCard.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "GP3_Team02/Task Management/FGTaskComponent.h"

AFGCardScanner::AFGCardScanner()
{	
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));	
	BoxComponent->SetupAttachment(RootComponent);
}

void AFGCardScanner::Interact(UFGInteractableLineTraceComponent* LineTraceComponent)
{
	Super::Interact(LineTraceComponent);
	UE_LOG(LogTemp,Warning,TEXT("Interact"));
	OpenDoor();
}

void AFGCardScanner::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFGCardScanner::HandleOverlap);
}

void AFGCardScanner::OpenDoor()
{	
	if (Unlocked == false) return;

	if(CompleteTaskComponentOnOpen)
		TaskComponent->CompleteTaskComponent();
	
	BP_OpenDoor();
}

void AFGCardScanner::ScanKeyCard(AFGCard* Card)
{
	if (Card == nullptr) return;

	if (Card->PassWord == PassWord)
	{
		if(CompleteTaskComponentOnUnlock)
			TaskComponent->CompleteTaskComponent();
		
		Unlocked = true;
		BP_ScanKeyCard();
	}	
}

void AFGCardScanner::HandleOverlap(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{
	UE_LOG(LogTemp,Warning, TEXT("OverLapped"));
	AFGCard* Card = Cast<AFGCard>(OtherActor);

	ScanKeyCard(Card);
}
