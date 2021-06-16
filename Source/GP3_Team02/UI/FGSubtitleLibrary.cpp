#include "FGSubtitleLibrary.h"

#include "FGHUD.h"
#include "FGViewportWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"


void UFGSubtitleLibrary::DisplaySubtitles(UObject* WorldContextObject, FText SubtitleText, float DurationInSeconds)
{
	AFGHUD* Hud = UGameplayStatics::GetPlayerController(WorldContextObject, 0)->GetHUD<AFGHUD>();
	if(Hud != nullptr)
		Hud->ViewportWidget->SetSubtitles(SubtitleText, DurationInSeconds);
	else
		UE_LOG(LogTemp, Warning, TEXT("Hud was nullptr (in FGSubtitleLibrary)"));
}

void UFGSubtitleLibrary::DisplaySubtitlesFragmented(UObject* WorldContextObject, TArray<FText> SubtitleText,
	TArray<float> DurationInSeconds)
{
	AFGHUD* Hud = UGameplayStatics::GetPlayerController(WorldContextObject, 0)->GetHUD<AFGHUD>();
	if(Hud != nullptr)
		Hud->ViewportWidget->SetSubtitlesFragmented(SubtitleText, DurationInSeconds);
	else
		UE_LOG(LogTemp, Warning, TEXT("Hud was nullptr (in FGSubtitleLibrary)"));
}

void UFGSubtitleLibrary::SetShowSubtitles(UObject* WorldContextObject, bool State)
{
	AFGHUD* Hud = UGameplayStatics::GetPlayerController(WorldContextObject, 0)->GetHUD<AFGHUD>();
	if(Hud != nullptr)
	{
		Hud->ViewportWidget->ShowingSubtitlesSetting = State;

		if(!State)
			Hud->ViewportWidget->ClearSubtitles();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Hud was nullptr (in FGSubtitleLibrary)"));
}
