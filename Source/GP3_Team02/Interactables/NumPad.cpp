#include "NumPad.h"
#include "FGInteractable.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NumPadButton.h"

class FInteractDelegate;

// Sets default values
ANumPad::ANumPad()
{
	PrimaryActorTick.bCanEverTick = true;
	Root 			= CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;	
	Mesh 			= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TaskComponent	= CreateDefaultSubobject<UFGTaskComponent>(TEXT("TaskComponent"));
	
	RedButton		= CreateDefaultSubobject<UChildActorComponent>(TEXT("RedButton"));
	GreenButton     = CreateDefaultSubobject<UChildActorComponent>(TEXT("GreenButton"));
	BlueButton      = CreateDefaultSubobject<UChildActorComponent>(TEXT("BlueButton"));
	YellowButton    = CreateDefaultSubobject<UChildActorComponent>(TEXT("YellowButton"));

	Mesh->SetupAttachment(RootComponent);
	
	RedButton		->SetupAttachment(Root);
	GreenButton		->SetupAttachment(Root);
	BlueButton		->SetupAttachment(Root);
	YellowButton	->SetupAttachment(Root);
}

void ANumPad::BeginPlay()
{
	Super::BeginPlay();
	
	if (RedButton)
	{
		ANumPadButton* NumPadButton = Cast<ANumPadButton>(RedButton->GetChildActor());							
		NumPadButton->OnInteract.AddDynamic(this, &ANumPad::RedButtonInteract);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Numpad Not Connected to RedButton"));
	}
	
	if (GreenButton)
	{
		ANumPadButton* NumPadButton = Cast<ANumPadButton>(GreenButton->GetChildActor());							
		NumPadButton->OnInteract.AddDynamic(this, &ANumPad::GreenButtonInteract);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Numpad Not Connected to GreenButton"));
	}
	
	if (BlueButton)
	{
		ANumPadButton* NumPadButton = Cast<ANumPadButton>(BlueButton->GetChildActor());							
		NumPadButton->OnInteract.AddDynamic(this, &ANumPad::BlueButtonInteract);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Numpad Not Connected to BlueButton"));
	}
	
	if (YellowButton)
	{
		ANumPadButton* NumPadButton = Cast<ANumPadButton>(YellowButton->GetChildActor());							
		NumPadButton->OnInteract.AddDynamic(this, &ANumPad::YellowButtonInteract);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("Numpad Not Connected to YellowButton"));
	}
}

void ANumPad::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

bool ANumPad::CheackCode()
{	
	for (int i = 0; i < InputColors.Num(); i++)
	{
		if (CodeColors[i] != InputColors[i])
		{
			return false;
		}		
		UE_LOG(LogTemp, Warning, TEXT("%i"), InputColors[i]);
	}
	BP_LockSolved();
	Completed.Broadcast(true);
	return true;
}

void ANumPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentIndex > InputColors.Num() - 1)
	{
		CurrentIndex = 0;

		ANumPadButton* RedNumPadButton 		= Cast<ANumPadButton>(RedButton->GetChildActor());
		ANumPadButton* GreenNumPadButton 	= Cast<ANumPadButton>(GreenButton->GetChildActor());
		ANumPadButton* BlueNumPadButton		= Cast<ANumPadButton>(BlueButton->GetChildActor());
		ANumPadButton* YellowNumPadButton 	= Cast<ANumPadButton>(YellowButton->GetChildActor());
		
		RedNumPadButton		-> ResetNumPad();
		GreenNumPadButton	-> ResetNumPad();
		BlueNumPadButton	-> ResetNumPad();
		YellowNumPadButton	-> ResetNumPad();

		for (int i = 0; i < InputColors.Num(); i++)
		{
			InputColors[i] = Colors::Default;	
		}
	}
}

void ANumPad::RedButtonInteract(AFGInteractable* interact)
{
	UE_LOG(LogTemp, Warning, TEXT("RedButtonInteract"));
	InputColors[CurrentIndex] = Colors::RedButton;
	CurrentIndex++;
	CheackCode();
	BP_InteractSound();
}

void ANumPad::GreenButtonInteract(AFGInteractable* interact)
{	
	UE_LOG(LogTemp, Warning, TEXT("GreenButtonInteract"));
	InputColors[CurrentIndex] = Colors::GreenButton;
	CurrentIndex++;
	CheackCode();
	BP_InteractSound();
}

void ANumPad::BlueButtonInteract(AFGInteractable* interact)
{	
	UE_LOG(LogTemp, Warning, TEXT("BlueButtonInteract"));
	InputColors[CurrentIndex] = Colors::BlueButton;
	CurrentIndex++;
	CheackCode();
	BP_InteractSound();
}

void ANumPad::YellowButtonInteract(AFGInteractable* interact)
{	
	UE_LOG(LogTemp, Warning, TEXT("YellowButtonInteract"));
	InputColors[CurrentIndex] = Colors::YellowButton;
	CurrentIndex++;
	CheackCode();
	BP_InteractSound();
}

