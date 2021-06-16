// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GP3_Team02/Interactables/FGInteractable.h"

#include "CrainButton.generated.h"

class ACrainActor;

//Diffrent functions the buttons will have.
UENUM()
enum Directions
{
	Left,
	Right,
	Up,
	Down,
	Forwards,
	Backwards,
	Stop,
	TurnOffOnMagnet
};

UCLASS()
class GP3_TEAM02_API ACrainButton : public AFGInteractable
{
	GENERATED_BODY()
	public:
	ACrainButton();

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;
	
	void BeginPlay() override;
	void Interact(UFGInteractableLineTraceComponent* LineTraceComponent);

	UPROPERTY(EditAnywhere)
	ACrainActor* CrainActor;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Directions> MovementDirection;
};


