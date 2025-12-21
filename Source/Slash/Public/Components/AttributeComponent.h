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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void ReceiveDamage(float Damage);
	float GetHealthPercentage() const;
	bool IsAlive() const;
	FORCEINLINE int32 GetSouls() const { return Soul; }
	FORCEINLINE int32 GetGold() const { return Gold; }
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
	int32 Soul = 0;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	int32 Gold = 0;
};
