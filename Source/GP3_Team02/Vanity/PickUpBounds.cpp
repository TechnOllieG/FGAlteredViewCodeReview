#include "PickUpBounds.h"
#include "GP3_Team02/FGGameStateBase.h"
#include "Kismet/GameplayStatics.h"

APickUpBounds::APickUpBounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void APickUpBounds::BeginPlay()
{
	Super::BeginPlay();
	
	Box->OnComponentBeginOverlap.AddDynamic(this,&APickUpBounds::OnOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this,&APickUpBounds::EndOverlap);
}

void APickUpBounds::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpBounds::OnOverlap(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHit)
{
	if (bUseEndOverLap)
		return;
	
	AFGPickupInteractable* Pickup = Cast<AFGPickupInteractable>(OtherActor);

	if (Pickup == nullptr)
	{
		return;
	}

	Pickup->DropItem();
	Pickup->SetActorLocation(Pickup->StartPosition);
	Pickup->SetActorRotation(Pickup->StartRotation);

	UPrimitiveComponent* PickupPrimitiveComponent = Pickup->FindComponentByClass<UPrimitiveComponent>();

	if (PickupPrimitiveComponent)
	{
		PickupPrimitiveComponent->SetSimulatePhysics(false);
		PickupPrimitiveComponent->SetSimulatePhysics(true);
	}
	


	UE_LOG(LogTemp, Warning, TEXT("OnOverlap"));
}

void APickUpBounds::EndOverlap(UPrimitiveComponent* OverLappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bUseEndOverLap)
		return;

	AFGPickupInteractable* Pickup = Cast<AFGPickupInteractable>(OtherActor);

	if (Pickup == nullptr)
	{
		return;
	}

	Pickup->DropItem();
	Pickup->SetActorLocation(Pickup->StartPosition);
	Pickup->SetActorRotation(Pickup->StartRotation);

	UPrimitiveComponent* PickupPrimitiveComponent = Pickup->FindComponentByClass<UPrimitiveComponent>();

	if (PickupPrimitiveComponent)
	{
		PickupPrimitiveComponent->SetSimulatePhysics(false);
		PickupPrimitiveComponent->SetSimulatePhysics(true);
	}
	
}
