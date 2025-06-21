// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AItem::BeginPlay"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 60.0f, FColor::Cyan, TEXT("AItem::BeginPlay OnScreenDebugMessage"));
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

