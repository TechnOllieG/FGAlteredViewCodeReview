#include "FGTaskTablet.h"

#include "FGTaskTabletUIBase.h"
#include "Components/WidgetComponent.h"

AFGTaskTablet::AFGTaskTablet()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComponent->SetupAttachment(RootComponent);
}

UFGTaskTabletUIBase* AFGTaskTablet::GetWidget() const
{
	WidgetComponent->InitWidget();
	return Cast<UFGTaskTabletUIBase>(WidgetComponent->GetWidget());
}
