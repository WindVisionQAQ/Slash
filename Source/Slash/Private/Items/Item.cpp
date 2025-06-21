// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"

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
		// Add debug info in c++ code that can be seen in PIE.
		GEngine->AddOnScreenDebugMessage(1, 60.0f, FColor::Cyan, TEXT("AItem::BeginPlay OnScreenDebugMessage"));
	}
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetActorForwardVector() * 100.f;
	DRAW_SPHERE(GetActorLocation());
	DRAW_VECTOR(StartLocation, EndLocation);
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
}

