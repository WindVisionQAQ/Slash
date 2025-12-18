// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

USlashOverlay* ASlashHUD::GetSlashOverlay() const
{
	return SlashOverlayInst;
}

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();
	InitSlashOverlay();
}

void ASlashHUD::InitSlashOverlay()
{
	APlayerController* Controller = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;
	if (SlashOverlayClass && Controller)
	{
		SlashOverlayInst = CreateWidget<USlashOverlay>(Controller, SlashOverlayClass);
		if (SlashOverlayInst)
		{
			SlashOverlayInst->AddToViewport();
		}
	}
}
