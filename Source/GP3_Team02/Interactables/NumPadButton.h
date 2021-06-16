// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGInteractable.h"
#include "NumPadButton.generated.h"

class ANumPad;
enum Colors;

/**
 * 
 */
UCLASS()
class GP3_TEAM02_API ANumPadButton : public AFGInteractable
{
	GENERATED_BODY()
	public:
	ANumPadButton();
	void Tick(float DeltaSeconds) override;
	void BeginPlay() override;
	virtual void Interact(UFGInteractableLineTraceComponent* LineTraceComponent);
	void ResetNumPad();

	UPROPERTY(EditDefaultsOnly)	
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<Colors> InteractColor;

	bool CanInteract = true;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere)
	UMaterial* ResetMaterial;

	UPROPERTY(EditAnywhere)
	float TimeToChange = 0.25;

	float CurrentTime = 0;	
};

