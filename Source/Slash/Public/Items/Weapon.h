// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USceneComponent;
class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();
	void Equip(USceneComponent* AttachComponent, FName AttachSocketName, AActor* Owner, APawn* Instigator, bool bPlaySound = true);
	void AttachMeshToSocket(USceneComponent* AttachComponent, FName AttachSocketName);
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	FORCEINLINE TArray<AActor*> GetActorsToIgnoreDuringBoxTrace() const { return ActorsToIgnoreDuringBoxTrace; }
	FORCEINLINE void ClearActorsToIgnore() { ActorsToIgnoreDuringBoxTrace.Reset(); }
protected:
	virtual void BeginPlay() override;
	void PlayEquipSound();
	void DisableSphereCollision();
	void DeactivateEmbers();
	void ExecuteGetHit(const FHitResult& BoxHitResult);
	bool IsIgnoreWeaponOverlapOrHit(AActor* OtherActor);

	UFUNCTION()
	void OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& Position);
private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bDebugBoxTrace = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Box Trace")
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Box Trace")
	USceneComponent* BoxTraceEnd;

	TArray<AActor*> ActorsToIgnoreDuringBoxTrace;

};
