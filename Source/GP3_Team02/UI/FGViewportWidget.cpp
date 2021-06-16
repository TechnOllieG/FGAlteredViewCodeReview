#include "FGViewportWidget.h"

#include "Kismet/GameplayStatics.h"

void UFGViewportWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!CurrentlyDisplayingSubtitles)
		return;
	
	float CurrentTime = UGameplayStatics::GetTimeSeconds(this);

	if(CurrentTime - TimeWhenSubtitlesWasSet > CurrentDuration)
	{
		if(CurrentSubtitleIsFragmented)
		{
			if(FragmentedSubtitleText.Num() > CurrentSubtitleIndex)
			{
				BP_SetSubtitleText(FragmentedSubtitleText[CurrentSubtitleIndex]);
				TimeWhenSubtitlesWasSet = UGameplayStatics::GetTimeSeconds(this);
				CurrentDuration = FragmentedSubtitleDuration[CurrentSubtitleIndex];
				CurrentSubtitleIndex++;
				return;
			}
		}
		
		ClearSubtitles();
	}
}

void UFGViewportWidget::SetSubtitles(const FText& SubtitleText, float DurationInSecs)
{
	if(!ShowingSubtitlesSetting)
		return;
	
	BP_SetSubtitleText(SubtitleText);
	TimeWhenSubtitlesWasSet = UGameplayStatics::GetTimeSeconds(this);
	CurrentDuration = DurationInSecs;
	CurrentlyDisplayingSubtitles = true;
	
	CurrentSubtitleIsFragmented = false;
	FragmentedSubtitleText.Empty();
	FragmentedSubtitleDuration.Empty();
}

void UFGViewportWidget::SetSubtitlesFragmented(const TArray<FText>& SubtitleText, TArray<float> DurationInSecs)
{
	if(!ShowingSubtitlesSetting)
		return;
	
	if(SubtitleText.Num() == 0 || FragmentedSubtitleDuration.Num() == 0)
	{
		UE_LOG(LogSubtitle, Warning, TEXT("Tried to set a fragmented subtitle with one of the arrays being empty"));
	}
	
	BP_SetSubtitleText(SubtitleText[0]);
	TimeWhenSubtitlesWasSet = UGameplayStatics::GetTimeSeconds(this);
	CurrentDuration = DurationInSecs[0];
	CurrentlyDisplayingSubtitles = true;

	CurrentSubtitleIndex = 1;
	CurrentSubtitleIsFragmented = true;
	FragmentedSubtitleText = SubtitleText;
	FragmentedSubtitleDuration = DurationInSecs;
}

void UFGViewportWidget::ClearSubtitles()
{
	BP_ClearSubtitles();
	TimeWhenSubtitlesWasSet = -999.f;
	CurrentDuration = 0.f;
	CurrentlyDisplayingSubtitles = false;
		
	CurrentSubtitleIsFragmented = false;
	FragmentedSubtitleText.Empty();
	FragmentedSubtitleDuration.Empty();
}
