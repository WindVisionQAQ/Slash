// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPickInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API IPickInterface
{
	GENERATED_BODY()
public:
	virtual void SetOverlappingItem(class AItem* Item);
	virtual void AddSoul(class ASoul* Soul);
	virtual void AddGold(class ATreasure* Treasure);
};
