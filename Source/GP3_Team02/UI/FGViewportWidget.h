#pragma once
#include "Blueprint/UserWidget.h"
#include "FGViewportWidget.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UFGViewportWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_SetSubtitleText(const FText& SubtitleText);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ClearSubtitles();

	void SetSubtitles(const FText& SubtitleText, float DurationInSecs);

	void SetSubtitlesFragmented(const TArray<FText>& SubtitleText, TArray<float> DurationInSecs);

	bool ShowingSubtitlesSetting = true;

	void ClearSubtitles();

private:
	float TimeWhenSubtitlesWasSet = -999.f;
	float CurrentDuration = 0.f;
	bool CurrentlyDisplayingSubtitles = false;
	bool CurrentSubtitleIsFragmented = false;
	int CurrentSubtitleIndex = 0;
	TArray<FText> FragmentedSubtitleText;
	TArray<float> FragmentedSubtitleDuration;
};