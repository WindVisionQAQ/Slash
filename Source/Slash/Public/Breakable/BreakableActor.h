// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

class USoundBase;
class UGeometryCollectionComponent;
class UCapsuleComponent;
class ATreasure;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBreak(const FChaosBreakEvent& BreakEvent);
private:	
	UPROPERTY(VisibleAnywhere, Category = GeometryCollection)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* PawnBlockCapsule;

	UPROPERTY(EditAnywhere, Category = SoundEffects)
	USoundBase* BreakSoundEffects;

	UPROPERTY(EditDefaultsOnly, Category = "Breakable Properties")
	TArray<TSubclassOf<ATreasure>> SpawnedTreasureClasses;

	bool bIsBroken = false;
};
