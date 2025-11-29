// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActor.generated.h"

class USoundBase;
class UGeometryCollectionComponent;

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
private:
	UPROPERTY(EditAnywhere, Category = SoundEffects)
	USoundBase* BreakSoundEffects;
};
