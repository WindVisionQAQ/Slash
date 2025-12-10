// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/SlashCharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarWidgetComponent;
class AAIController;
class UPawnSensingComponent;
class AWeapon;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	UFUNCTION()
	void HandlePawnSeen(APawn* SeenPawn);
	void Attack();
	virtual void Destroyed() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
private:
	void MoveToActor(AActor* TargetActor);
	bool IsNearTargetActor(AActor* TargetActor, float DistanceThreshold);
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void RefreshPatrolPoint();
	void MoveToNewPatrolPoint();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	void StartAttackTimer();
	void ClearPatrolTimer();
	void ClearAttackTimer();

	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
protected:

	UPROPERTY(EditAnywhere, Category = "Combat")
	float CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRadius = 200.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DeathLifeSpan = 3.f;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeadPose> DeadPose;
private:

	UPROPERTY(VisibleAnywhere, Category = "Health")
	UHealthBarWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolPoints;

	UPROPERTY()
	AActor* CurrentPatrolPoint;

	// If the distance between enemy and current patrol point is less or equal than this value, refresh and pick a new patrol point.
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolRefreshDistance = 300.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChaseSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY()
	AAIController* EnemyController = nullptr;

	FTimerHandle PatrolTimerHandle;

public:


};
