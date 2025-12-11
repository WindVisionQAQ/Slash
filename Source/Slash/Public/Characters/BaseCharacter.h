// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UParticleSystem;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	virtual void BeginPlay() override;
	virtual bool CanAttack();
	virtual void Die();
	bool IsAlive();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void DisableCapsuleCollision();
	void DisableMeshCollision();
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	/**
	* Play Animation montage function
	*/
	void PlayHitMontage(FName SectionName);
	virtual int32 PlayDeathMontage();
	int32 PlayAttackMontage();
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	

	void DirectionalHitReaction(const FVector& ImpactPoint);
public:

protected:
	UPROPERTY(VisibleAnywhere, Category = Combat)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathSections;
	/**
	 * Animation Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadOnly)
	AActor* CombatTarget;

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

};
