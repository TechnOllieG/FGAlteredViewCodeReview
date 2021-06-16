// Fill out your copyright notice in the Description page of Project Settings.


#include "NumPadButton.h"

#include "NumPad.h"

ANumPadButton::ANumPadButton()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh 	= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh	->SetCollisionProfileName("Interactable");
	RootComponent = Mesh;
}

void ANumPadButton::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetMaterial(0,DefaultMaterial);
}

void ANumPadButton::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (CanInteract == false)
	{
		Mesh->SetMaterial(0,ResetMaterial);
		CurrentTime += DeltaSeconds;

		if (CurrentTime >= TimeToChange)
		{
			Mesh->SetMaterial(0,DefaultMaterial);
			CurrentTime = 0;
			CanInteract = true;
		}
	}
}

void ANumPadButton::ResetNumPad()
{
	CanInteract = false;
}

void ANumPadButton::Interact(UFGInteractableLineTraceComponent* LineTraceComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));

	ResetNumPad();
	
	OnInteract.Broadcast(this);	
	BP_Interact();
}
