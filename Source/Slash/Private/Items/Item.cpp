// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/StaticMeshComponent.h"	
#include "Components/SphereComponent.h"
#include "Characters/SlashCharacter.h"
#include "NiagaraComponent.h"
#include "Sound/SoundBase.h"
#include "Interface/PickInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetupAttachment(GetRootComponent());

	EmberComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Ember"));
	EmberComponent->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComp)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
		SphereComp->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	}

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

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickInterface* PickInterface = Cast<IPickInterface>(OtherActor);
	if (PickInterface)
	{
		PickInterface->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickInterface* PickInterface = Cast<IPickInterface>(OtherActor);
	if (PickInterface)
	{
		PickInterface->SetOverlappingItem(NULL);
	}
}

void AItem::SpawnPickupSound()
{
	if (PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation());
	}
}

void AItem::SpawnPickupEffect()
{
	if (PickUpEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickUpEffect, GetActorLocation());
	}
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

