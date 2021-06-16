// Fill out your copyright notice in the Description page of Project Settings.


#include "NumPadDoor.h"
#include "NumPad.h"

ANumPadDoor::ANumPadDoor()
{
	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	
	NumPad		= CreateDefaultSubobject<UChildActorComponent>(TEXT("NumPad"));
	NumPad		->SetupAttachment(RootComponent);

	DoorFrame	->SetupAttachment(SceneComponent);
	LeftDoor	->SetupAttachment(DoorFrame);
	Lock		->SetupAttachment(LeftDoor);
	RightDoor	->SetupAttachment(DoorFrame);
	NumPad		->SetupAttachment(Lock);
}

void ANumPadDoor::BeginPlay()
{
	Super::BeginPlay();
	NumPadChiled = Cast<ANumPad>(NumPad->GetChildActor());

	if (NumPadChiled)
	{
		NumPadChiled->Completed.AddDynamic(this, &ANumPadDoor::NumPadCompleted);

		for (int i = 0;i < Code.Num();i++)
		{
			if (i > NumPadChiled->CodeColors.Num() - 1)
			{
				NumPadChiled->CodeColors.Add(Code[i]);
			}
			else
			{
				NumPadChiled->CodeColors[i] = Code[i];
			}			
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Cast to NumPadChiled succsesful"));	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to NumPadChiled failed"));	
	}
}

void ANumPadDoor::NumPadCompleted(bool Completed)
{
	if (Completed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Completed true"));	
		BP_OpenDoor();	
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Completed false"));	
	}

}
