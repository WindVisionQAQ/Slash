// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/StaticMeshComponent.h"	

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AItem::BeginPlay"));
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(TimeConstant * RunningTime);
}

float AItem::TransformedCos()
{
	return Amplitude* FMath::Cos(TimeConstant * RunningTime);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GEngine)
	{
		FString ItemName = GetName();
		FString LogMessage = FString::Printf(TEXT("AItem::Tick DeltaTime=%f ItemName=%s"), DeltaTime, *ItemName);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage);
		GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Purple, LogMessage);
	}

	RunningTime += DeltaTime;
}

