// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();
	void ReceiveDamage(float Damage);
	void UseStamina(float InStaminaCost);
	void RegenStamina(float DeltaTime);
	float GetHealthPercentage() const;
	float GetStaminaPercent() const;
	bool IsAlive() const;
	FORCEINLINE int32 GetSouls() const { return Soul; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetStamina() const { return Stamina; }
	FORCEINLINE int32 GetDodgetCost() const { return DodgeCost; }
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);
protected:
	virtual void BeginPlay() override;

private:
	// Current health
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float DodgeCost = 14.f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float StaminaRegenRate = 4.f;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 Soul = 0;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 Gold = 0;
};
