// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interface/PickInterface.h"

void ASoul::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickInterface* PickInterface = Cast<IPickInterface>(OtherActor);
	if (PickInterface)
	{
		PickInterface->AddSoul(this);
		SpawnPickupSound();
		SpawnPickupEffect();
		Destroy();
	}
}
