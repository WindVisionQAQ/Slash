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
	virtual void GetHit_Implementation(const FVector& ImpactPoint, const AActor* HitInstigator) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
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
	void PlayHitMontage(FName SectionName);
	virtual int32 PlayDeathMontage();
	int32 PlayAttackMontage();
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	void DirectionalHitReaction(const FVector& ImpactPoint);
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
protected:
	UPROPERTY(VisibleAnywhere, Category = Combat)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathSections;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadOnly)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitEffects;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	UAttributeComponent* AttributeComp;

};
