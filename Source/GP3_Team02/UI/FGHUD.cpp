#include "FGHUD.h"

#include "FGViewportWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AFGHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	ViewportWidget = CreateWidget<UFGViewportWidget, APlayerController>(Controller, ViewportWidgetClass, TEXT("Subtitle Widget"));
	ViewportWidget->BP_ClearSubtitles();
	ViewportWidget->AddToViewport();
}
