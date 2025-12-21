// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/PickInterface.h"

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickInterface* PickInterface = Cast<IPickInterface>(OtherActor);
	if (PickInterface)
	{
		PickInterface->AddGold(this);
		SpawnPickupSound();
		SpawnPickupEffect();
		Destroy();
	}
}
