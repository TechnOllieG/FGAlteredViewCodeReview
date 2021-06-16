#pragma once
#include "FGSubtitleLibrary.generated.h"

UCLASS()
class UFGSubtitleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", UnsafeDuringActorConstruction = true))
	static void DisplaySubtitles(UObject* WorldContextObject, FText SubtitleText, float DurationInSeconds);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", UnsafeDuringActorConstruction = true))
	static void DisplaySubtitlesFragmented(UObject* WorldContextObject, TArray<FText> SubtitleText, TArray<float> DurationInSeconds);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", UnsafeDuringActorConstruction = true))
	static void SetShowSubtitles(UObject* WorldContextObject, bool State);
};