// Copyright Yerik Guo


#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (OverlayWidgetClass)
	{
		OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
		if (OverlayWidget)
		{
			OverlayWidget->AddToViewport();
		}
	}
}
