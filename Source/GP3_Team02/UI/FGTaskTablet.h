#pragma once
#include "GameFramework/Actor.h"
#include "FGTaskTablet.generated.h"

class UFGTaskTabletUIBase;
class UWidgetComponent;
UCLASS()
class AFGTaskTablet : public AActor
{
	GENERATED_BODY()

public:
	AFGTaskTablet();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UWidgetComponent* WidgetComponent;

	UFUNCTION(BlueprintCallable)
	UFGTaskTabletUIBase* GetWidget() const;
};