// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarWidgetComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComp->SetPeripheralVisionAngle(45.f);
	PawnSensingComp->SightRadius = 2000.f;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else if (EnemyState > EEnemyState::EES_Dead)
	{
		CheckPatrolTarget();
	}

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	ShowHealthBar();
}

float AEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(Damage);
	if (EventInstigator && CombatTarget != EventInstigator->GetPawn())
	{
		CombatTarget = EventInstigator->GetPawn();
		ChaseTarget();
		ClearPatrolTimer();
	}
	return Damage;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyController = Cast<AAIController>(GetController());
	HideHealthBar();
	if (!CurrentPatrolPoint) RefreshPatrolPoint();
	StartPatrolling();
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::HandlePawnSeen);
	}
	InitWeapon();
	Tags.Add("Enemy");
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::Die()
{
	EnemyState = EEnemyState::EES_Dead;
	ClearAttackTimer();
	PlayDeathMontage();
	DisableCapsuleCollision();
	DisableMeshCollision();
	HideHealthBar();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	const bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (HealthBarWidgetComponent && AttributeComp)
	{
		HealthBarWidgetComponent->SetHealthPercentage(AttributeComp->GetHealthPercentage());
	}
}

int32 AEnemy::PlayDeathMontage()
{
	const int32 SectionIndex = Super::PlayDeathMontage();
	TEnumAsByte<EDeadPose> Pose(SectionIndex);
	if (SectionIndex >= 0 && Pose < EDeadPose::EDP_MAX)
	{
		DeadPose = Pose;
	}
	return SectionIndex;
}

void AEnemy::MoveToActor(AActor* TargetActor)
{
	if (!EnemyController || !TargetActor) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(TargetActor);
	MoveRequest.SetAcceptanceRadius(30.f);
	EnemyController->MoveTo(MoveRequest);
}

bool AEnemy::IsNearTargetActor(AActor* TargetActor, float DistanceThreshold)
{
	if (!TargetActor) return false;
	return (GetActorLocation() - TargetActor->GetActorLocation()).Size() <= DistanceThreshold;
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (IsNearTargetActor(CurrentPatrolPoint, PatrolRefreshDistance))
	{
		RefreshPatrolPoint();
		GetWorldTimerManager().SetTimer(PatrolTimerHandle, this, &AEnemy::MoveToNewPatrolPoint, 5.f, false);
	}
}

void AEnemy::RefreshPatrolPoint()
{
	TArray<AActor*> TempPatrolPoints;
	for (AActor* PatrolPoint : PatrolPoints)
	{
		if (PatrolPoint != CurrentPatrolPoint)
		{
			TempPatrolPoints.AddUnique(PatrolPoint);
		}
	}
	if (TempPatrolPoints.Num())
	{
		const int32 RandomIndex = FMath::RandRange(0, TempPatrolPoints.Num() - 1);
		CurrentPatrolPoint = TempPatrolPoints[RandomIndex];
	}
}

void AEnemy::MoveToNewPatrolPoint()
{
	MoveToActor(CurrentPatrolPoint);
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	MoveToActor(CurrentPatrolPoint);
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	MoveToActor(CombatTarget);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !IsNearTargetActor(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !IsNearTargetActor(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return IsNearTargetActor(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::InitWeapon()
{
	if (WeaponClass)
	{
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
		if (EquippedWeapon)
		{
			EquippedWeapon->Equip(GetMesh(), EquippedWeapon->GetItemArmAttachSocketName(), this, this, false);
		}
	}
}

void AEnemy::HandlePawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		!IsDead() &&
		!IsChasing() &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn &&
		SeenPawn->ActorHasTag("EngagableActor");

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}
