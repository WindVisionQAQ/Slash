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

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
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
	virtual void Die() override;
	UFUNCTION()
	void HandlePawnSeen(APawn* SeenPawn);
private:
	void MoveToActor(AActor* TargetActor);
	bool IsNearTargetActor(AActor* TargetActor, float DistanceThreshold);
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void RefreshPatrolPoint();
	void MoveToNewPatrolPoint();
protected:

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float AlertDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Enemy Properties")
	float AttackRadius = 300.f;
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
	float PatrolSpeed = 75.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChaseSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY()
	AAIController* EnemyController = nullptr;

	FTimerHandle PatrolTimerHandle;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

public:


};
