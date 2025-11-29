// Fill out your copyright notice in the Description page of Project Settings.


#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Items/Treasure.h"

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
	PawnBlockCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PawnBlockCapsule"));
	if (PawnBlockCapsule)
	{
		PawnBlockCapsule->SetupAttachment(GetRootComponent());
		PawnBlockCapsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		PawnBlockCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
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
	// Trigger OnBreak only once to prevent multiple treasures to spawn
	if (bIsBroken) return;
	bIsBroken = true;

	if (PawnBlockCapsule)
	{
		PawnBlockCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	}
	UWorld* World = GetWorld();
	if (World && SpawnedTreasureClass)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;
		World->SpawnActor<ATreasure>(SpawnedTreasureClass, Location, GetActorRotation());
	}
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
	}
}


