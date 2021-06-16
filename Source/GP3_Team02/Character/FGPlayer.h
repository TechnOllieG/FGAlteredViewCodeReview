#pragma once
#include "GameFramework/Character.h"
#include "FGPlayer.generated.h"

class UFGInteractableLineTraceComponent;
class UCameraComponent;

UCLASS()
class GP3_TEAM02_API AFGPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AFGPlayer();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	bool EnableJump = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	UFGInteractableLineTraceComponent* LineTraceComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnStartWalking();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnStopWalking();

private:
	void HandleMoveForward(float Value);
	void HandleMoveRight(float Value);
	void HandleLookUp(float Value);
	void HandleLookRight(float Value);
	void HandleJumpInput();

	float ForwardMovementInput;
	float RightMovementInput;
	bool CurrentlyWalking = false;
};
