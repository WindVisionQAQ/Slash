// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarWidgetComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidgetComponent::SetHealthPercentage(float Percentage)
{
	if (HealthBarWidgetInstance == nullptr)
	{
		HealthBarWidgetInstance = Cast<UHealthBarWidget>(GetUserWidgetObject());
	}
	if (HealthBarWidgetInstance && HealthBarWidgetInstance->HealthProgressBar)
	{
		HealthBarWidgetInstance->HealthProgressBar->SetPercent(Percentage);
	}
}
