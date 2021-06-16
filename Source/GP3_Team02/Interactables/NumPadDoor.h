// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGCardScanner.h"
#include "NumPadDoor.generated.h"

class FANumPadCompleted;
class ANumPad;

enum Colors;

UCLASS()
class GP3_TEAM02_API ANumPadDoor : public AFGCardScanner
{
	GENERATED_BODY()
	public:
	ANumPadDoor();
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* DoorFrame;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* LeftDoor;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* Lock;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UStaticMeshComponent* RightDoor;	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UChildActorComponent* NumPad;

	void BeginPlay() override;
	ANumPad* NumPadChiled;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<Colors>> Code;

	UFUNCTION()
	void NumPadCompleted(bool Completed);
};
