// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	if (GeometryCollection)
	{
		GeometryCollection->SetGenerateOverlapEvents(true);
		GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		GeometryCollection->SetNotifyBreaks(true);
	}
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (GeometryCollection)
	{
		GeometryCollection->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::OnBreak);
	}
}

void ABreakableActor::OnBreak(const FChaosBreakEvent& BreakEvent)
{
	SetLifeSpan(3.0f);
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (BreakSoundEffects)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BreakSoundEffects, ImpactPoint);
		GeometryCollection->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
}


