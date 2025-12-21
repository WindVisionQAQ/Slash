// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/SlashCharacterTypes.h"
#include "Item.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundBase;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped,
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	template <typename T>
	T Avg(T First, T Second);

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SpawnPickupSound();
	void SpawnPickupEffect();

public:
	FORCEINLINE ECharacterState GetItemCharacterStateOnEquipped() const { return CharacterStateOnEquipped; }
	FORCEINLINE FName GetItemArmAttachSocketName() const { return ItemArmAttachSocketName; }
	FORCEINLINE FName GetItemDisarmAttachSocketName() const { return ItemDisarmAttachSocketName; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equip)
	ECharacterState CharacterStateOnEquipped = ECharacterState::ECS_EquippedOneHandedWeapon;

	UPROPERTY(EditDefaultsOnly)
	FName ItemArmAttachSocketName = TEXT("RightHandSocket");

	UPROPERTY(EditDefaultsOnly)
	FName ItemDisarmAttachSocketName = TEXT("SpineSocket");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere, Category = "Visual Effects")
	UNiagaraComponent* EmberComponent;

	UPROPERTY(EditAnywhere, Category = "Item")
	UNiagaraSystem* PickUpEffect;

	UPROPERTY(EditAnywhere, Category = "Item")
	USoundBase* PickUpSound;

private:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;
};

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
