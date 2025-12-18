// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

class USlashOverlay;

/**
 * 
 */
UCLASS()
class SLASH_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

public:
	USlashOverlay* GetSlashOverlay() const;
protected:
	virtual void BeginPlay() override;
private:
	void InitSlashOverlay();

	USlashOverlay* SlashOverlayInst = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USlashOverlay> SlashOverlayClass;
};
