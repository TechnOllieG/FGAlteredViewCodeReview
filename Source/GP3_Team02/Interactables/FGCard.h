// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FGInteractable.h"
#include "FGPickupInteractable.h"

#include "FGCard.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class GP3_TEAM02_API AFGCard : public AFGPickupInteractable
{
	GENERATED_BODY()
	
public:
	AFGCard();
	
	UPROPERTY(EditAnywhere)
	FName PassWord = "Sus";
};
