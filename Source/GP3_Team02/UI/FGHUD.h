#pragma once
#include "GameFramework/HUD.h"
#include "FGHUD.generated.h"

class UFGViewportWidget;

UCLASS()
class AFGHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGViewportWidget> ViewportWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UFGViewportWidget* ViewportWidget;
};