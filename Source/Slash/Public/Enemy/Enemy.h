// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarWidgetComponent;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;
	void Die();
private:
	/**
	* Play Animation montage function
	*/
	void PlayHitMontage(FName SectionName);
	void PlayDeathMontage(FName SectionName);

	void DirectionalHitReaction(const FVector& ImpactPoint);

protected:
	UPROPERTY(BlueprintReadOnly)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float AlertDistance = 500.f;
private:
	/**
	 * Animation Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	/**
	 * Sound Resources
	 */
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	/**
	 * Visual Effects
	 */
	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitEffects;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	UAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	UHealthBarWidgetComponent* HealthBarWidgetComponent;

public:


};
