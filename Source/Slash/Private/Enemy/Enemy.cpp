// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarWidgetComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	if (GetMesh())
	{
		GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetGenerateOverlapEvents(true);
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
	HealthBarWidgetComponent = CreateDefaultSubobject<UHealthBarWidgetComponent>(TEXT("HealthBarComp"));
	HealthBarWidgetComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetHealthPercentage(1.f);
		HealthBarWidgetComponent->SetVisibility(false);
	}
	GetCharacterMovement()->MaxWalkSpeed = 75.f;
	EnemyController = Cast<AAIController>(GetController());
	if (!CurrentPatrolPoint) RefreshPatrolPoint();
	if (EnemyController && CurrentPatrolPoint)
	{
		MoveToActor(CurrentPatrolPoint);
	}
}

void AEnemy::Die()
{
	if (DeathMontage)
	{
		const int32 DeathMontageSectionNum = DeathMontage->GetNumSections();
		const int32 Selection = FMath::RandRange(1, DeathMontageSectionNum);
		FName SectionName = FName(*FString::Printf(TEXT("Death%d"), Selection));
		PlayDeathMontage(SectionName);
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (HealthBarWidgetComponent)
	{
		HealthBarWidgetComponent->SetVisibility(false);
	}
	SetLifeSpan(5.f);
}

void AEnemy::PlayHitMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	}
}

void AEnemy::PlayDeathMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}

void AEnemy::DirectionalHitReaction(const FVector& ImpactPoint)
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector ToHit = ImpactPoint - GetActorLocation();
	const FVector ToHit_SameHeight = FVector(ToHit.X, ToHit.Y, ForwardVector.Z).GetSafeNormal();
	const double DotProduct_ToHitForward = FVector::DotProduct(ForwardVector, ToHit_SameHeight);
	const FVector CrossProduct_ToHitForward = FVector::CrossProduct(ToHit_SameHeight, ForwardVector);
	double Theta = FMath::RadiansToDegrees(FMath::Acos(DotProduct_ToHitForward));
	if (CrossProduct_ToHitForward.Z < 0)
	{
		Theta *= -1;
	}
	FName SectionName = FName("FromBack");
	if (Theta > -45.f && Theta <= 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if (Theta > -135.f && Theta <= -45.f)
	{
		SectionName = FName("FromRight");
	}
	else if (Theta > 45.f && Theta <= 135.f)
	{
		SectionName = FName("FromLeft");
	}
	PlayHitMontage(SectionName);
}

void AEnemy::MoveToActor(AActor* TargetActor)
{
	if (!EnemyController || !TargetActor) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(TargetActor);
	MoveRequest.SetAcceptanceRadius(100.f);
	EnemyController->MoveTo(MoveRequest);
}

bool AEnemy::IsNearTargetActor(AActor* TargetActor, float DistanceThreshold)
{
	if (!TargetActor) return false;
	return (GetActorLocation() - TargetActor->GetActorLocation()).Size() <= DistanceThreshold;
}

void AEnemy::CheckCombatTarget()
{
	if (!CombatTarget) return;
	if (!IsNearTargetActor(CombatTarget, AlertDistance))
	{
		if (HealthBarWidgetComponent)
		{
			HealthBarWidgetComponent->SetVisibility(false);
		}
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (IsNearTargetActor(CurrentPatrolPoint, PatrolRefreshDistance))
	{
		RefreshPatrolPoint();
		MoveToActor(CurrentPatrolPoint);
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


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckCombatTarget();
	CheckPatrolTarget();
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (AttributeComp)
	{
		AttributeComp->ReceiveDamage(Damage);
		if (HealthBarWidgetComponent)
		{
			HealthBarWidgetComponent->SetHealthPercentage(AttributeComp->GetHealthPercentage());
			HealthBarWidgetComponent->SetVisibility(true);
		}
		if (EventInstigator)
		{
			CombatTarget = EventInstigator->GetPawn();
		}
	}
	return Damage;
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	if (AttributeComp && AttributeComp->IsAlive())
	{
		DirectionalHitReaction(ImpactPoint);
	}
	else
	{
		Die();
	}
	
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitEffects && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffects, ImpactPoint);
	}
}

