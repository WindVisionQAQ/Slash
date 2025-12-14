// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AttributeComp = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->ClearActorsToIgnore();
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::Die()
{

}

bool ABaseCharacter::IsAlive()
{
	return AttributeComp && AttributeComp->IsAlive();
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitEffects && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffects, ImpactPoint);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (AttributeComp)
	{
		AttributeComp->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::DisableCapsuleCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::PlayHitMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(HitMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	}
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathSections);
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackSections);
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (Montage == nullptr || SectionNames.Num() <= 0) return -1;
	const int MaxSectionIndex = SectionNames.Num() - 1;
	const int SectionIndex = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[SectionIndex]);
	return SectionIndex;
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ABaseCharacter::DirectionalHitReaction(const FVector& ImpactPoint)
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

void ABaseCharacter::AttackEnd()
{

}

