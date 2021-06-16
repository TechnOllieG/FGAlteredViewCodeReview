#include "FGPlayer.h"

#include "FGInteractableLineTraceComponent.h"
#include "Camera/CameraComponent.h"

AFGPlayer::AFGPlayer()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	LineTraceComponent = CreateDefaultSubobject<UFGInteractableLineTraceComponent>(TEXT("Interactable Line Trace"));
	PrimaryActorTick.bCanEverTick = true;
}

void AFGPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AFGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFGPlayer::HandleMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFGPlayer::HandleMoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFGPlayer::HandleLookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AFGPlayer::HandleLookRight);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, LineTraceComponent, &UFGInteractableLineTraceComponent::StartInteract);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Released, LineTraceComponent, &UFGInteractableLineTraceComponent::EndInteract);
	
	if(EnableJump)
		PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFGPlayer::HandleJumpInput);
}

void AFGPlayer::HandleJumpInput()
{
	Jump();
}


void AFGPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AddMovementInput((ForwardMovementInput * GetActorForwardVector() + RightMovementInput * GetActorRightVector()).GetSafeNormal());

	if(FMath::IsNearlyZero(ForwardMovementInput) && FMath::IsNearlyZero(RightMovementInput))
	{
		if(CurrentlyWalking)
		{
			CurrentlyWalking = false;
			BP_OnStopWalking();
		}
	}
	else
	{
		if(!CurrentlyWalking && !Controller->IsMoveInputIgnored())
		{
			CurrentlyWalking = true;
			BP_OnStartWalking();
		}
	}
}

void AFGPlayer::HandleMoveForward(float Value)
{
	ForwardMovementInput = Value;
}

void AFGPlayer::HandleMoveRight(float Value)
{
	RightMovementInput = Value;
}

void AFGPlayer::HandleLookUp(float Value)
{
	if(!Controller->IsMoveInputIgnored())
		AddControllerPitchInput(Value);
}

void AFGPlayer::HandleLookRight(float Value)
{
	if(!Controller->IsMoveInputIgnored())
		AddControllerYawInput(Value);
}